#include "StageBattleManager.hpp"

namespace {
// EnemyActor fields read here (offsets into the guest object; the class itself is another module).
namespace ea {
constexpr uint32_t POSITION = 624;        // cocos2d::Vec2 map position (MapNode)
constexpr uint32_t STATE = 672;           // int: 3 = active on the field, 6 = calm/inactive
constexpr uint32_t STATUS = 680;          // SafeBuffer: data +4 = max HP, +8 = HP
constexpr uint32_t STATUS_DATA = 692;     //   its data pointer
constexpr uint32_t BADSTATUS = 712;       // object with a virtual "find bad status" at slot 0 (sret, this, id)
constexpr uint32_t PARAM = 728;           // std::shared_ptr<SafeBuffer> (data +0 = enemy id for sub mission 12)
constexpr uint32_t APPROACH_OFFSET = 744; // float, front offset used by firstApproachIsMoveToFront
constexpr uint32_t HOME = 764;            // cocos2d::Vec2 home position
constexpr uint32_t CAN_MOVE_FRONT = 789;  // uint8
constexpr uint32_t ACTION = 816;          // pointer; its +1312 = current action kind
constexpr uint32_t ACTION_KIND = 1312;
constexpr uint32_t HP_INDICATOR = 960;    // RaidBossHPIndicator*
}  // namespace ea

namespace g {
constexpr uint32_t ref_ctor = rwlib::cocos2d::Ref::ctor_base;               // cocos2d::Ref::Ref()
constexpr uint32_t aeabi_memset = 0x40E7A0;           // __aeabi_memset(dst, n, c)
constexpr uint32_t vector_clear = 0x74ED1D;           // cocos2d::Vector<EnemyActor*>::clear()
constexpr uint32_t vector_dtor = 0x74EAF7;            // cocos2d::Vector<EnemyActor*>::~Vector()
constexpr uint32_t vector_pushBack = 0x74F0F5;        // cocos2d::Vector<EnemyActor*>::pushBack(EnemyActor*)
constexpr uint32_t enemy_isDied = 0x6008ED;           // EnemyActor::isDied() const
constexpr uint32_t enemy_iniApproachMove = 0x74C8B3;  // EnemyActor::iniApproachMove()
constexpr uint32_t enemy_getScreenTouchRect = 0x74C7A9;  // EnemyActor::getScreenTouchRect() (sret Rect)
constexpr uint32_t enemy_setApproachMove = 0x74C8BD;  // EnemyActor::setApproachMove(Vec2&, bool)
constexpr uint32_t enemy_exeApproachMove = 0x74C9DD;  // EnemyActor::exeApproachMove(float, const Vec2&, bool)
constexpr uint32_t enemy_endedSelectWait = 0x74D9A5;  // EnemyActor::endedSelectWait()
constexpr uint32_t enemy_setupSelectWait = 0x74D8DD;  // EnemyActor::setupSelectWait(const std::function<void(EnemyActor*,bool)>&)
constexpr uint32_t enemy_startSelectWait = 0x74D981;  // EnemyActor::startSelectWait()
constexpr uint32_t enemy_isSetSelectWait = 0x74DA25;  // EnemyActor::isSetSelectWait()
constexpr uint32_t enemy_ceaseSelectWait = 0x74D999;  // EnemyActor::ceaseSelectWait()
constexpr uint32_t enemy_actionMarker = 0x74A65D;     // EnemyActor::actionMarker(TapAction)
constexpr uint32_t badstatus_create = 0x779525;       // master::Badstatus::create(int) (sret shared_ptr)
constexpr uint32_t random_getInstance = 0x492055;     // Random::getInstance()
constexpr uint32_t random_uniformInt = 0x492225;      // Random::generateUniformInt(int, int, unsigned*)
constexpr uint32_t hpIndicator_setBufDebuf = 0x5E7D91;  // RaidBossHPIndicator::set_buf_debuf(buff_idx, int)
constexpr uint32_t add_shared = 0xC8DBB5;             // std::__shared_weak_count::__add_shared()
constexpr uint32_t appState_getInstance = 0x4E66DD;   // AppState::getInstance()
constexpr uint32_t stageState_cmpSubMission12 = 0x4EFBD5;  // StageState::cmpSubMission12(int)
constexpr uint32_t frameWidth = 0x799B31;             // AppGetGrameFrameWidth()
constexpr uint32_t frameHeight = 0x799D21;            // AppGetGrameFrameHeight()
constexpr uint32_t size_ctor = rwlib::cocos2d::Size::ctor_base__ff;              // cocos2d::Size::Size(float, float)
constexpr uint32_t size_equals = rwlib::cocos2d::Size::equals;            // cocos2d::Size::equals(const Size&) const
constexpr uint32_t rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;              // cocos2d::Rect::Rect(float, float, float, float)
constexpr uint32_t rect_union = rwlib::cocos2d::Rect::unionWithRect;             // cocos2d::Rect::unionWithRect(const Rect&) const (sret)
constexpr uint32_t rect_intersects = rwlib::cocos2d::Rect::intersectsRect;        // cocos2d::Rect::intersectsRect(const Rect&) const
constexpr uint32_t rect_midX = rwlib::cocos2d::Rect::getMidX;              // cocos2d::Rect::getMidX() const
constexpr uint32_t rect_midY = rwlib::cocos2d::Rect::getMidY;              // cocos2d::Rect::getMidY() const
constexpr uint32_t vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;              // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;              // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;              // cocos2d::Vec2::~Vec2()
constexpr uint32_t vec2_set = rwlib::cocos2d::Vec2::set__RKS0_S2_;               // cocos2d::Vec2::set(const Vec2&, const Vec2&)
constexpr uint32_t vec2_isZero = rwlib::cocos2d::Vec2::isZero;            // cocos2d::Vec2::isZero() const
constexpr uint32_t vec2_subtract = rwlib::cocos2d::Vec2::subtract;          // cocos2d::Vec2::subtract(const Vec2&)
constexpr uint32_t vec2_length = rwlib::cocos2d::Vec2::length;            // cocos2d::Vec2::length() const
constexpr uint32_t vec2_ZERO = 0x1006950;             // cocos2d::Vec2::ZERO
constexpr uint32_t stageField_isHitTestTouchRectAndTouchUI = 0x757E75;  // StageFieldManager::isHitTestTouchRectAndTouchUI(EnemyActor*)
constexpr uint32_t callFunc_create = rwlib::cocos2d::CallFunc::create;        // cocos2d::CallFunc::create(const std::function<void()>&)
constexpr uint32_t moveBy_create = rwlib::cocos2d::MoveBy::create;          // cocos2d::MoveBy::create(float, const Vec2&)
constexpr uint32_t easeSineOut_create = rwlib::cocos2d::EaseSineOut::create;     // cocos2d::EaseSineOut::create(ActionInterval*)
constexpr uint32_t sequence_create = rwlib::cocos2d::Sequence::create;        // cocos2d::Sequence::create(FiniteTimeAction*, ...)
constexpr uint32_t mapNode_move = 0x5DA415;           // MapNode::move(cocos2d::Action*)
// std::function heap-less functor vtables (lambdas bound to an actor / the manager)
constexpr uint32_t fn_frontBegin = 0xF6E150;          // firstApproachIsMoveToFront lambda #1 (avatar)
constexpr uint32_t fn_frontMoved = 0xF6E124;          // lambda #2 (avatar)
constexpr uint32_t fn_frontEnd = 0xF6E0F8;            // lambda #3 (avatar)
constexpr uint32_t fn_selectWait = 0xF6E0CC;          // startSelectedTargetEnemy lambda (manager)
}  // namespace g

uint32_t state(uint32_t enemy) { return rw::load<uint32_t>(enemy + ea::STATE); }
uint32_t isDied(uint32_t enemy) { return rw::call(g::enemy_isDied, {enemy}); }   // EnemyActor::isDied

// Build a std::function<...> holding a lambda that captured one pointer, in place at guest address `fn`
// (16-byte buffer, the callable pointer at +16 points back into the buffer).
void makeFunction(const bt::GuestFrame &frame, uint32_t off, uint32_t vtable, uint32_t capture) {
    frame.setWord(off + 16, frame.addr(off));
    frame.setWord(off, vtable);
    frame.setWord(off + 4, capture);
}
// std::function destructor: in-place callable -> destroy() (slot 4), heap callable -> destroy_deallocate() (slot 5).
void destroyFunction(const bt::GuestFrame &frame, uint32_t off) {
    uint32_t f = frame.word(off + 16);
    if (f == frame.addr(off)) rw::vcall(frame.ptr<void>(off), 4, {f});
    else if (f) rw::vcall(rw::host_ptr<void>(f), 5, {f});
}
}  // namespace

StageBattleManager *StageBattleManager::create() {
    auto *self = rw::host_ptr<StageBattleManager>(rw::call(bt::fn::operator_new, {0x30}));
    construct(self);
    if (rw::vcall(self, 2, {rw::arg(self)}) == 1) {          // init()
        rw::call(bt::fn::ref_autorelease, {rw::arg(self)});   // cocos2d::Ref::autorelease
        return self;
    }
    rw::vcall(self, 1, {rw::arg(self)});                       // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x5FC830, "StageBattleManager::create", StageBattleManager::create);

StageBattleManager *StageBattleManager::construct(StageBattleManager *self) {
    rw::call(g::ref_ctor, {rw::arg(self)});                   // cocos2d::Ref::Ref
    self->vtable = VTABLE;
    rw::call(g::aeabi_memset, {rw::arg(self) + 24, 0x18, 0}); // attackers, selectEnemies, target, timer
    return self;
}
RW_FUNCTION(0x75376C, "StageBattleManager::StageBattleManager", StageBattleManager::construct);

StageBattleManager *StageBattleManager::destruct(StageBattleManager *self) {
    self->vtable = VTABLE;
    uint32_t v = rw::arg(&self->attackers);
    rw::call(g::vector_clear, {v});                           // cocos2d::Vector<EnemyActor*>::clear
    rw::call(g::vector_dtor, {v});                            // cocos2d::Vector<EnemyActor*>::~Vector
    rw::call(bt::fn::ref_dtor, {rw::arg(self)});              // cocos2d::Ref::~Ref
    return self;
}
RW_FUNCTION(0x7537A0, "StageBattleManager::~StageBattleManager", StageBattleManager::destruct);

void StageBattleManager::destructDelete(StageBattleManager *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {rw::arg(self)});       // operator delete
}
RW_FUNCTION(0x7537FC, "StageBattleManager::~StageBattleManager(deleting)", StageBattleManager::destructDelete);

uint32_t StageBattleManager::init(StageBattleManager *) { return 1; }
RW_FUNCTION(0x753808, "StageBattleManager::init", StageBattleManager::init);

// All enemies dead (an empty team counts as annihilated).
uint32_t StageBattleManager::isEnemyTeamAnnihilation(StageBattleManager *, bt::Vector *enemies) {
    uint32_t it = enemies->begin, end = enemies->end;
    for (; it != end; it += 4)
        if (isDied(rw::load<uint32_t>(it)) == 0) return 0;
    return 1;
}
RW_FUNCTION(0x75380C, "StageBattleManager::isEnemyTeamAnnihilation", StageBattleManager::isEnemyTeamAnnihilation);

// Every enemy is calm: inactive (state 6) or doing an action of kind 0..2.
uint32_t StageBattleManager::isEnemyTeamCalmDown(StageBattleManager *, bt::Vector *enemies) {
    uint32_t begin = enemies->begin, end = enemies->end;
    int32_t calm = 0;
    for (uint32_t it = begin; it != end; it += 4) {
        uint32_t e = rw::load<uint32_t>(it);
        if (state(e) == 6) { calm++; continue; }
        uint32_t kind = rw::load<uint32_t>(rw::load<uint32_t>(e + ea::ACTION) + ea::ACTION_KIND);
        if (kind <= 1 || kind == 2) calm++;
    }
    return (int32_t(end - begin) >> 2) == calm ? 1 : 0;
}
RW_FUNCTION(0x753834, "StageBattleManager::isEnemyTeamCalmDown", StageBattleManager::isEnemyTeamCalmDown);

// Collect this turn's attackers (active enemies not stopped by bad status 1; bad status 2 = paralysis holds
// an enemy back unless it resists, chance from master Badstatus 122), then shuffle them.
void StageBattleManager::setEnemyAttacker(StageBattleManager *self, bt::Vector *enemies) {
    uint32_t list = rw::arg(&self->attackers);
    rw::call(g::vector_clear, {list});                        // cocos2d::Vector<EnemyActor*>::clear
    self->attackerCount = 0;
    uint32_t it = enemies->begin, end = enemies->end;
    if (it == end) return;
    bt::GuestFrame frame(0x20);                               // +0 shared_ptr<Badstatus>, +8 bad status result
    const uint32_t bs = frame.addr(0), found = frame.addr(8);
    do {
        uint32_t e = rw::load<uint32_t>(it);
        if (state(e) == 3) {
            uint32_t holder = e + ea::BADSTATUS;
            rw::vcall(rw::host_ptr<void>(holder), 0, {found, holder, 1});
            if (frame.word(8 + 8) == 0) {
                rw::vcall(rw::host_ptr<void>(holder), 0, {found, holder, 2});
                bool attack = frame.word(8 + 8) == 0;
                if (!attack) {
                    rw::call(g::badstatus_create, {bs, 122});        // master::Badstatus::create
                    int32_t resist = rw::host_ptr<SafeBuffer>(frame.word(0))->get32(84);
                    uint32_t rnd = rw::call(g::random_getInstance, {});  // Random::getInstance
                    int32_t roll = int32_t(rw::call(g::random_uniformInt, {rnd, 1, 10000, 0}));
                    if (resist < roll) {
                        rw::call(g::hpIndicator_setBufDebuf,          // RaidBossHPIndicator::set_buf_debuf
                                 {rw::load<uint32_t>(e + ea::HP_INDICATOR), 18, 21});
                        attack = true;
                    }
                    bt::releaseShared(frame.word(4));
                }
                if (attack) {
                    rw::call(g::vector_pushBack, {list, e});          // cocos2d::Vector<EnemyActor*>::pushBack
                    self->attackerCount = self->attackerCount + 1;
                }
            }
        }
        it += 4;
    } while (end != it);
    if (self->attackerCount < 2) return;
    int32_t k = 0;
    do {
        uint32_t rnd = rw::call(g::random_getInstance, {});
        uint32_t a = rw::call(g::random_uniformInt, {rnd, 0, uint32_t(self->attackerCount - 1), 0});
        rnd = rw::call(g::random_getInstance, {});
        uint32_t b = rw::call(g::random_uniformInt, {rnd, 0, uint32_t(self->attackerCount - 1), 0});
        uint32_t base = self->attackers.begin;
        uint32_t pa = base + 4 * a, pb = base + 4 * b;
        uint32_t va = rw::load<uint32_t>(pa), vb = rw::load<uint32_t>(pb);
        rw::save<uint32_t>(pa, vb);
        rw::save<uint32_t>(pb, va);
        k++;
    } while (k < self->attackerCount);
}
RW_FUNCTION(0x753890, "StageBattleManager::setEnemyAttacker", StageBattleManager::setEnemyAttacker);

// No attackers left and every living enemy is idle (action kinds 0, 1, 2 or 5).
uint32_t StageBattleManager::IsAllOverEnemyAttker(StageBattleManager *self, bt::Vector *enemies) {
    if (self->attackerCount > 0) return 0;
    for (uint32_t it = enemies->begin, end = enemies->end; it != end; it += 4) {
        uint32_t e = rw::load<uint32_t>(it);
        if (isDied(e) != 0) continue;
        uint32_t kind = rw::load<uint32_t>(rw::load<uint32_t>(e + ea::ACTION) + ea::ACTION_KIND);
        if (kind > 5 || ((1u << kind) & 0x27) == 0) return 0;
    }
    return 1;
}
RW_FUNCTION(0x753A20, "StageBattleManager::IsAllOverEnemyAttker", StageBattleManager::IsAllOverEnemyAttker);

// Lowest HP / max HP among living enemies (1.0 when none).
float StageBattleManager::MinHPRateEnemyTeam(StageBattleManager *, bt::Vector *enemies) {
    float lowest = 1.0f;
    for (uint32_t it = enemies->begin, end = enemies->end; it != end; it += 4) {
        uint32_t e = rw::load<uint32_t>(it);
        if (isDied(e) != 0) continue;
        const SafeBuffer *status = rw::host_ptr<SafeBuffer>(e + ea::STATUS);
        int32_t hp = status->get32(8);
        int32_t maxHp = status->get32(4);
        float rate = float(hp) / float(maxHp);
        if (rate < lowest) lowest = rate;
    }
    return lowest;
}
RW_FUNCTION(0x753A78, "StageBattleManager::MinHPRateEnemyTeam", StageBattleManager::MinHPRateEnemyTeam);

uint32_t StageBattleManager::hasEnemyAttker(StageBattleManager *self) { return self->attackerCount > 0 ? 1 : 0; }
RW_FUNCTION(0x753B68, "StageBattleManager::hasEnemyAttker", StageBattleManager::hasEnemyAttker);

// Pop the next living attacker (from the back), 0 when none is left.
uint32_t StageBattleManager::getEnemyAttcker(StageBattleManager *self) {
    int32_t n = self->attackerCount;
    if (n < 1) return 0;
    do {
        self->attackerCount = n - 1;
        uint32_t base = self->attackers.begin;
        int32_t size = int32_t(self->attackers.end.get() - base) >> 2;
        if (n > size) {
            n = n - 1;
        } else {
            uint32_t e = rw::load<uint32_t>(base + 4 * uint32_t(n - 1));
            if (isDied(e) != 1) return e;
            n = self->attackerCount;
        }
    } while (n > 0);
    return 0;
}
RW_FUNCTION(0x753B78, "StageBattleManager::getEnemyAttcker", StageBattleManager::getEnemyAttcker);

void StageBattleManager::clearEnemyAttker(StageBattleManager *self) { self->attackerCount = 0; }
RW_FUNCTION(0x753BBE, "StageBattleManager::clearEnemyAttker", StageBattleManager::clearEnemyAttker);

void StageBattleManager::initApproachEnemyTeam(StageBattleManager *self, bt::Vector *enemies) {
    for (uint32_t it = enemies->begin, end = enemies->end; it != end; it += 4) {
        uint32_t e = rw::load<uint32_t>(it);
        if (state(e) == 3) rw::call(g::enemy_iniApproachMove, {e});   // EnemyActor::iniApproachMove
    }
    self->approachTimer = 3.0f;
}
RW_FUNCTION(0x753BC4, "StageBattleManager::initApproachEnemyTeam", StageBattleManager::initApproachEnemyTeam);

// Move active enemies toward the target: an enemy off screen (or under the touch UI) heads for the screen centre,
// an enemy overlapping another one steps away from it (only after the approach timer ran out).
void StageBattleManager::approachEnemyTeam(StageBattleManager *self, float dt, uint32_t target, bt::Vector *enemies,
                                           uint32_t fieldManager) {
    // guest frame offsets as in the original
    enum : uint32_t { OTHER_RECT = 0x48, CENTER_E = 0x58, CENTER_S = 0x60, UNION = 0x68, UNION_SIZE = 0x70,
                      DIR = 0x78, RECT = 0x80, SCREEN = 0x90, SCREEN_SIZE = 0xA0 };
    bt::GuestFrame frame(0xB0);
    uint32_t w = rw::call(g::frameWidth, {});                 // AppGetGrameFrameWidth
    uint32_t h = rw::call(g::frameHeight, {});                // AppGetGrameFrameHeight
    rw::call(g::size_ctor, {frame.addr(SCREEN_SIZE), w, h});  // cocos2d::Size::Size
    uint32_t touched = 0;
    rw::call(g::rect_ctor, {frame.addr(SCREEN), 0, 0, frame.word(SCREEN_SIZE), frame.word(SCREEN_SIZE + 4)});
    uint32_t it = enemies->begin, end = enemies->end;
    if (it == end) {
        self->approachTimer = 3.0f;
    } else {
        do {
            uint32_t e = rw::load<uint32_t>(it);
            if (state(e) == 3) {
                rw::call(g::enemy_getScreenTouchRect, {frame.addr(RECT), e});
                rw::call(g::vec2_copy, {frame.addr(DIR), g::vec2_ZERO});
                rw::call(g::rect_union, {frame.addr(UNION), frame.addr(SCREEN), frame.addr(RECT)});
                // the union is larger than the screen = the enemy is partly off screen
                uint32_t hit = rw::call(g::size_equals, {frame.addr(SCREEN_SIZE), frame.addr(UNION_SIZE)}) ^ 1;
                if (hit == 0)
                    hit = rw::call(g::stageField_isHitTestTouchRectAndTouchUI, {fieldManager, e});
                touched |= hit;
                uint32_t move;
                bool setMove = false;
                if (hit == 1) {
                    uint32_t mx = rw::call(g::rect_midX, {frame.addr(SCREEN)});
                    uint32_t my = rw::call(g::rect_midY, {frame.addr(SCREEN)});
                    rw::call(g::vec2_ctor, {frame.addr(CENTER_S), mx, my});
                    mx = rw::call(g::rect_midX, {frame.addr(RECT)});
                    my = rw::call(g::rect_midY, {frame.addr(RECT)});
                    rw::call(g::vec2_ctor, {frame.addr(CENTER_E), mx, my});
                    rw::call(g::vec2_set, {frame.addr(DIR), frame.addr(CENTER_E), frame.addr(CENTER_S)});
                    frame.setWord(DIR + 4, bt::bits(-0.0f - bt::fromBits(frame.word(DIR + 4))));
                    rw::call(g::vec2_dtor, {frame.addr(CENTER_E)});
                    rw::call(g::vec2_dtor, {frame.addr(CENTER_S)});
                    setMove = true;
                } else {
                    move = 0;
                    for (uint32_t o = enemies->begin, oend = enemies->end; o != oend; o += 4) {
                        uint32_t other = rw::load<uint32_t>(o);
                        if (state(other) != 3) continue;
                        if (e == other) {
                            float t = self->approachTimer;
                            if (t > 0.0f || std::isnan(t)) break;   // move = 0
                            continue;
                        }
                        rw::call(g::enemy_getScreenTouchRect, {frame.addr(OTHER_RECT), other});
                        if (rw::call(g::rect_intersects, {frame.addr(RECT), frame.addr(OTHER_RECT)}) == 1) {
                            rw::call(g::vec2_set, {frame.addr(DIR), other + ea::POSITION, e + ea::POSITION});
                            rw::call(g::vec2_dtor, {frame.addr(OTHER_RECT)});
                            setMove = true;
                            break;
                        }
                        rw::call(g::vec2_dtor, {frame.addr(OTHER_RECT)});
                    }
                }
                if (setMove) {
                    if (rw::call(g::vec2_isZero, {frame.addr(DIR)}) == 1)
                        rw::call(g::vec2_set, {frame.addr(DIR), e + ea::POSITION, e + ea::HOME});
                    rw::call(g::enemy_setApproachMove, {e, frame.addr(DIR), 0});
                    move = 1;
                }
                rw::call(g::enemy_exeApproachMove, {e, bt::bits(dt), target, move});
                rw::call(g::vec2_dtor, {frame.addr(UNION)});
                rw::call(g::vec2_dtor, {frame.addr(DIR)});
                rw::call(g::vec2_dtor, {frame.addr(RECT)});
            }
            it += 4;
        } while (it != end);
        if ((touched & 1) == 0) {
            self->approachTimer = 3.0f;
        } else {
            float t = self->approachTimer;
            if (t > 0.0f) self->approachTimer = t - dt;          // !(t <= 0 || NaN)
        }
    }
    rw::call(g::vec2_dtor, {frame.addr(SCREEN)});
}
RW_FUNCTION(0x753BF0, "StageBattleManager::approachEnemyTeam", StageBattleManager::approachEnemyTeam);

// First approach: when the avatar and the enemy face the same way (or the enemy is behind), the avatar moves
// in front of the enemy (MoveBy with ease, at most 3.33 s, 640 px/s) between three callbacks.
uint32_t StageBattleManager::firstApproachIsMoveToFront(StageBattleManager *, uint32_t avatar, uint32_t enemy) {
    if (enemy == 0 || rw::load<uint8_t>(enemy + ea::CAN_MOVE_FRONT) == 0) return 0;
    enum : uint32_t { FN_END = 0x28, FN_MOVED = 0x40, FN_BEGIN = 0x58, VEC = 0x20 };
    bt::GuestFrame frame(0x80);
    float avatarDir = bt::fromBits(rw::vcall(rw::host_ptr<void>(avatar), 11, {avatar}));
    float enemyDir = bt::fromBits(rw::vcall(rw::host_ptr<void>(enemy), 11, {enemy}));
    uint32_t v = rw::call(g::vec2_copy, {frame.addr(VEC), enemy + ea::POSITION});   // Vec2(enemy pos)
    rw::call(g::vec2_subtract, {v, avatar + ea::POSITION});                       // -= avatar pos
    float x = bt::fromBits(frame.word(VEC));
    float side = enemyDir * x;
    uint32_t result = 0;
    if (avatarDir == enemyDir || side < 0.0f) {
        float off = rw::load<float>(enemy + ea::APPROACH_OFFSET);
        frame.setWord(VEC, bt::bits(x - enemyDir * off));
        result = 1;
        frame.setWord(VEC + 4, bt::bits(-0.0f - bt::fromBits(frame.word(VEC + 4))));
        float len = bt::fromBits(rw::call(g::vec2_length, {frame.addr(VEC)}));
        makeFunction(frame, FN_BEGIN, g::fn_frontBegin, avatar);
        float t = len / 640.0f;
        uint32_t begin = rw::call(g::callFunc_create, {frame.addr(FN_BEGIN)});
        const float maxDur = bt::fromBits(0x40555555);   // 3.3333333
        float dur = t > maxDur ? maxDur : t;
        uint32_t move = rw::call(g::moveBy_create, {bt::bits(dur), frame.addr(VEC)});
        uint32_t ease = rw::call(g::easeSineOut_create, {move});
        makeFunction(frame, FN_MOVED, g::fn_frontMoved, avatar);
        uint32_t moved = rw::call(g::callFunc_create, {frame.addr(FN_MOVED)});
        makeFunction(frame, FN_END, g::fn_frontEnd, avatar);
        uint32_t ended = rw::call(g::callFunc_create, {frame.addr(FN_END)});
        uint32_t seq = rw::call(g::sequence_create, {begin, ease, moved, ended, 0});
        destroyFunction(frame, FN_END);
        destroyFunction(frame, FN_MOVED);
        destroyFunction(frame, FN_BEGIN);
        rw::call(g::mapNode_move, {avatar, seq});            // MapNode::move
    }
    rw::call(g::vec2_dtor, {frame.addr(VEC)});
    return result;
}
RW_FUNCTION(0x753ED8, "StageBattleManager::firstApproachIsMoveToFront",
            StageBattleManager::firstApproachIsMoveToFront);

// Start the select-target mode: with two or more active enemies they all wait for the player's tap (the
// first one starts); with fewer the only candidate is selected right away.
void StageBattleManager::startSelectedTargetEnemy(StageBattleManager *self, bt::Vector *enemies) {
    self->selectEnemies = rw::arg(enemies);
    uint32_t it = enemies->begin, end = enemies->end;
    uint32_t first = 0;
    if (it != end) {
        int32_t active = 0;
        do {
            uint32_t e = rw::load<uint32_t>(it);
            if (state(e) == 3) {
                if (first == 0) first = e;
                active++;
            }
            rw::call(g::enemy_endedSelectWait, {e});         // EnemyActor::endedSelectWait
            it += 4;
        } while (end != it);
        if (active >= 2) {
            bt::Vector *list = rw::host_ptr<bt::Vector>(self->selectEnemies);
            bt::GuestFrame frame(0x18);
            for (uint32_t p = list->begin, pend = list->end; p != pend; p += 4) {
                uint32_t e = rw::load<uint32_t>(p);
                if (state(e) != 3) continue;
                makeFunction(frame, 0, g::fn_selectWait, rw::arg(self));
                rw::call(g::enemy_setupSelectWait, {e, frame.addr(0)});
                destroyFunction(frame, 0);
            }
            rw::call(g::enemy_startSelectWait, {first});     // EnemyActor::startSelectWait
            self->selectedTarget = 0;
            return;
        }
    }
    self->selectedTarget = first;
}
RW_FUNCTION(0x754180, "StageBattleManager::startSelectedTargetEnemy", StageBattleManager::startSelectedTargetEnemy);

uint32_t StageBattleManager::isSelectedTargetEnemy(StageBattleManager *self) {
    if (self->selectedTarget == 0) return 0;
    bt::Vector *list = rw::host_ptr<bt::Vector>(self->selectEnemies);
    for (uint32_t it = list->begin, end = list->end; it != end; it += 4) {
        uint32_t e = rw::load<uint32_t>(it);
        if (rw::call(g::enemy_isSetSelectWait, {e}) == 1) rw::call(g::enemy_endedSelectWait, {e});
    }
    return 1;
}
RW_FUNCTION(0x7542A0, "StageBattleManager::isSelectedTargetEnemy", StageBattleManager::isSelectedTargetEnemy);

// Tap on a waiting enemy: select=true picks it (the others stop waiting), false only highlights it.
uint32_t StageBattleManager::selectedTargetEnemy(StageBattleManager *self, uint32_t enemy, uint32_t select) {
    if (self->selectedTarget != 0) return self->selectedTarget;
    bt::Vector *list = rw::host_ptr<bt::Vector>(self->selectEnemies);
    uint32_t it = list->begin, end = list->end;
    uint32_t action;
    if (select) {
        for (; end != it; it += 4) {
            uint32_t e = rw::load<uint32_t>(it);
            if (rw::call(g::enemy_isSetSelectWait, {e}) == 1) rw::call(g::enemy_ceaseSelectWait, {e});
        }
        self->selectedTarget = enemy;
        action = 1;
    } else {
        for (; end != it; it += 4) {
            uint32_t e = rw::load<uint32_t>(it);
            if (rw::call(g::enemy_isSetSelectWait, {e}) == 1) rw::call(g::enemy_actionMarker, {e, 0});
        }
        action = 3;
    }
    return rw::call(g::enemy_actionMarker, {enemy, action});    // EnemyActor::actionMarker
}
RW_FUNCTION(0x7542DA, "StageBattleManager::selectedTargetEnemy", StageBattleManager::selectedTargetEnemy);

// Sub mission 12 (defeat specific enemies): report each active enemy's id to the stage state.
void StageBattleManager::checkSubMission12(StageBattleManager *, bt::Vector *enemies) {
    for (uint32_t it = enemies->begin, end = enemies->end; it != end; it += 4) {
        uint32_t e = rw::load<uint32_t>(it);
        if (state(e) != 3) continue;
        uint32_t app = rw::call(g::appState_getInstance, {});  // AppState::getInstance
        uint32_t ctrl = rw::load<uint32_t>(e + ea::PARAM + 4);
        uint32_t param = rw::load<uint32_t>(e + ea::PARAM);
        uint32_t stageState = rw::load<uint32_t>(app + 0x20);
        if (ctrl) rw::call(g::add_shared, {ctrl});
        int32_t id = rw::host_ptr<SafeBuffer>(param)->get32(0);
        rw::call(g::stageState_cmpSubMission12, {stageState, uint32_t(id)});
        bt::releaseShared(ctrl);
    }
}
RW_FUNCTION(0x754350, "StageBattleManager::checkSubMission12", StageBattleManager::checkSubMission12);
