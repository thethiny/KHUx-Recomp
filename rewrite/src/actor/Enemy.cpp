#include "Enemy.hpp"

#include "FunctionThunks.hpp"

namespace {
namespace g {
constexpr uint32_t vfs_exists = 0x8DD565;            // bg::vfs::exists(const char*)
constexpr uint32_t fnVoid_swap = 0x4EE86D;           // std::function<void()>::swap(function&)
constexpr uint32_t dequeActFlag_pushBack = 0x5C40C9; // std::deque<FlashNode::ActFlag>::push_back(ActFlag&&)
constexpr uint32_t hitEffect_preload = 0x5D95F9;     // HitEffect::preloadResources(int)
constexpr uint32_t motionFiles = 0xF5D0BC;           // const char*[7]: lwf/character/enemy/%d/<motion>/<motion>.lwf
constexpr uint32_t str_bwaitFile = 0xE01848;         // "lwf/character/enemy/%d/bwait/bwait.lwf"
constexpr uint32_t str_atkSpFile = 0xE018BD;         // "lwf/character/enemy/%d/atkSp/atkSp.lwf"
constexpr uint32_t str_all = 0xE01818;               // "all"
constexpr uint32_t str_body = 0xE00D8B;              // "body"
constexpr uint32_t str_attack = 0xDFB8DD;            // "attack"
constexpr uint32_t str_center = 0xE0181C;            // "center"
constexpr uint32_t motionEndLambda = 0xF5D0E0;       // Enemy::Enemy $_0 (captures this)
}  // namespace g

constexpr uint32_t NODE_SET_CASCADE_OPACITY = 136;   // cocos2d::Node::setCascadeOpacityEnabled(bool)

uint32_t ga(const void *p) { return rw::guest_addr(p); }

// A StringBuffer (char[1024] + length) on the guest stack, emptied.
void clearStringBuffer(uint32_t sb) {
    rw::save<uint32_t>(sb + 1024, 0);
    rw::save<uint8_t>(sb, 0);
}
// Movie::SearchMovieInstance(name, recursive) with a temporary std::string
uint32_t searchChild(const bt::GuestFrame &f, uint32_t movie, uint32_t name, uint32_t len) {
    actor::makeString(f.addr(0), name, len);
    uint32_t r = rw::call(flashlwf::movie_searchMovieInstance, {movie, f.addr(0), 1});
    actor::freeString(f.addr(0));
    return r;
}
// (GetX(), -GetY()) of a movie
void moviePoint(uint32_t movie, float *x, float *y) {
    uint32_t xb = rw::call(flashlwf::movie_getX, {movie});
    uint32_t yb = rw::call(flashlwf::movie_getY, {movie});
    *x = rw::bitsf(xb);
    *y = rw::bitsf(yb ^ 0x80000000u);
}
}  // namespace

Enemy *Enemy::create(int32_t enemyId) {
    auto *self = rw::host_ptr<Enemy>(rw::call(bt::fn::operator_new, {SIZE}));
    construct(self, enemyId);
    rw::call(actor_fn::ref_autorelease, {ga(self)});
    return self;
}
RW_FUNCTION(0x5CBE24, "Enemy::create", Enemy::create);

Enemy *Enemy::construct(Enemy *self, int32_t enemyId) {
    FlashNode::construct(self, 0);
    self->vtable = VTABLE;
    self->enemyId = enemyId;
    self->motion = 7;
    self->battleShiftSet = 0;
    self->firstLoad = 1;
    self->attackPoint.x = 0.0f;                              // Vec2()
    self->attackPoint.y = 0.0f;
    self->centerPoint.x = 0.0f;
    self->centerPoint.y = 0.0f;
    self->battleShift.x = 0.0f;
    self->battleShift.y = 0.0f;
    rw::vcall(self, NODE_SET_CASCADE_OPACITY, {ga(self), 1});
    bt::GuestFrame f(0x420);
    enum : uint32_t { FN = 0, SB = 0x18 };
    fnthunk::makeLocal(f.addr(FN), g::motionEndLambda, {ga(self)});
    rw::call(g::fnVoid_swap, {f.addr(FN), ga(self) + CMD_MOTION_END});
    fnthunk::destroy(f.addr(FN));
    clearStringBuffer(f.addr(SB));
    rw::call(flashlwf::stringBuffer_fset, {f.addr(SB), g::str_bwaitFile, uint32_t(int32_t(self->enemyId))});
    self->hasBwait = uint8_t(rw::call(g::vfs_exists, {f.addr(SB)}));
    rw::call(flashlwf::stringBuffer_fset, {f.addr(SB), g::str_atkSpFile, uint32_t(int32_t(self->enemyId))});
    self->hasAttackSp = uint8_t(rw::call(g::vfs_exists, {f.addr(SB)}));
    alterMotion(self, WAIT);
    return self;
}
RW_FUNCTION(0x5CBE60, "Enemy::Enemy", Enemy::construct);

// Replace the LWF node with the motion's file (bwait / atkSp fall back when the enemy has none), hook the movie
// script commands to the CommandParser, start the movie, and record the movie points on the first wait / bwait.
void Enemy::alterMotion(Enemy *self, int32_t m) {
    if (self->lwfNode) rw::vcall(self, flashlwf::NODE_REMOVE_CHILD, {ga(self), self->lwfNode, 1});
    if (m == BWAIT || (m == ATTACK_SP && self->hasAttackSp == 0)) m = self->hasBwait;
    enum : uint32_t { STR = 0, FN = 0x10, SB = 0x28 };
    bt::GuestFrame f(0x430);
    clearStringBuffer(f.addr(SB));
    rw::call(flashlwf::stringBuffer_fset,
             {f.addr(SB), actor::ld(g::motionFiles + uint32_t(m) * 4), uint32_t(int32_t(self->enemyId))});
    self->lwfNode = rw::call(flashlwf::lwfNode_create, {f.addr(SB), 0});   // cocos2d::LWFNode::create
    rw::vcall(self, node_slot::ADD_CHILD, {ga(self), self->lwfNode});
    self->motion = m;
    uint32_t lwf = actor::ld(self->lwfNode + LWF_NODE_LWF);
    fnthunk::makeLocal(f.addr(FN), flashlwf::commandParserBind, {flashlwf::commandParser_parse, 0, ga(self) + 672});
    rw::call(flashlwf::lwfHandler_assign, {lwf + flashlwf::LWF_HANDLER, f.addr(FN)});
    fnthunk::destroy(f.addr(FN));
    actor::vcallAt(self->lwfNode, flashlwf::NODE_UPDATE, {self->lwfNode, 0});

    lwf = actor::ld(self->lwfNode + LWF_NODE_LWF);
    actor::makeString(f.addr(STR), flashlwf::STR_CHAR, 4);
    uint32_t movie = rw::call(flashlwf::lwf_searchMovieInstance, {lwf, f.addr(STR)});
    actor::freeString(f.addr(STR));
    if (movie && actor::ld(movie + flashlwf::MOVIE_FRAME) == 1) {
        actor::makeString(f.addr(STR), flashlwf::STR_START, 5);
        rw::call(flashlwf::movie_gotoAndPlay, {movie, f.addr(STR)});
        actor::freeString(f.addr(STR));
    }
    actor::vcallAt(self->lwfNode, flashlwf::NODE_UPDATE, {self->lwfNode, 0});

    if (self->firstLoad != 0 && self->motion == WAIT) {
        self->firstLoad = 0;
        if (movie) {
            self->attackPoint.y = actor::ldf(actor_fn::vec2_ZERO + 4);
            self->attackPoint.x = actor::ldf(actor_fn::vec2_ZERO);
            uint32_t all = searchChild(f, movie, g::str_all, 3);
            if (all) {
                float x, y;
                moviePoint(all, &x, &y);
                self->battleShift.y = y;
                self->battleShift.x = x;
                uint32_t body = searchChild(f, all, g::str_body, 4);
                if (body) {
                    uint32_t attack = searchChild(f, body, g::str_attack, 6);
                    if (attack) {
                        moviePoint(attack, &x, &y);
                        self->attackPoint.x = x;
                        self->attackPoint.y = y;
                    }
                    uint32_t center = searchChild(f, body, g::str_center, 6);
                    if (center) {
                        moviePoint(center, &x, &y);
                        self->centerPoint.y = y;
                        self->centerPoint.x = x;
                    }
                }
            }
        }
    }
    if (self->battleShiftSet == 0 && self->motion == BWAIT) {
        self->battleShiftSet = 1;
        if (movie) {
            uint32_t all = searchChild(f, movie, g::str_all, 3);
            if (all) {
                float x, y;
                moviePoint(all, &x, &y);
                float bx = self->battleShift.x, by = self->battleShift.y;   // Vec2::subtract
                self->battleShift.x = bx - x;
                self->battleShift.y = by - y;
            }
        }
    }
    self->field_270 = 0;
    self->freezeOnStatus = 0;
    if (m == KNOCKBACK || uint32_t(m) <= 2) {
        if (m != KNOCKBACK) self->freezeOnStatus = 1;       // wait / bwait / move stop while asleep / paralysed
        rw::set<float>(self, 644, 1.0f);                     // Vec4(1, 1, 1, 1)
        rw::set<float>(self, 640, 1.0f);
        rw::set<float>(self, 636, 1.0f);
        rw::set<float>(self, 632, 1.0f);
    }
    change_sub(self);
}
RW_FUNCTION(0x5CC06C, "Enemy::alterMotion", Enemy::alterMotion);

// Preload every motion's LWF and the hit effects.
void Enemy::preloadResources(const Enemy *self) {
    enum : uint32_t { STR = 0, SB = 0x10 };
    bt::GuestFrame f(0x420);
    clearStringBuffer(f.addr(SB));
    for (uint32_t k = 0; k < 7; k++) {
        rw::call(flashlwf::stringBuffer_fset,
                 {f.addr(SB), actor::ld(g::motionFiles + 4 * k), uint32_t(int32_t(self->enemyId))});
        actor::makeString(f.addr(STR), f.addr(SB), rw::call(actor_fn::strlen_, {f.addr(SB)}));
        rw::call(flashlwf::lwfUtil_preload, {f.addr(STR)});
        actor::freeString(f.addr(STR));
    }
    uint32_t it = self->hitEffectIds.begin, end = self->hitEffectIds.end;
    for (; it != end; it += 4) rw::call(g::hitEffect_preload, {actor::ld(it)});
}
RW_FUNCTION(0x5CC688, "Enemy::preloadResources", Enemy::preloadResources);

// The hit effect of the current attack: the second id for the special attack, else the first (1 by default).
int32_t Enemy::getHitEffectID(const Enemy *self) {
    uint32_t begin = self->hitEffectIds.begin;
    if (self->motion == ATTACK_SP) {
        uint32_t n = uint32_t(int32_t(self->hitEffectIds.end - begin) >> 2);
        if (n < 2) return 1;
        return actor::ldi(begin + 4);
    }
    if (begin == self->hitEffectIds.end) return 1;
    return actor::ldi(begin);
}
RW_FUNCTION(0x5CC788, "Enemy::getHitEffectID", Enemy::getHitEffectID);

uint32_t Enemy::getBattleShift(const Enemy *self) {
    return self->battleShiftSet ? ga(&self->battleShift) : actor_fn::vec2_ZERO;
}
RW_FUNCTION(0x5CC7C4, "Enemy::getBattleShift", Enemy::getBattleShift);

Enemy *Enemy::destruct(Enemy *self) {
    self->vtable = VTABLE;                                   // (the Vec2 destructors are empty)
    FlashNode::destruct(self);
    return self;
}
RW_FUNCTION(0x5CC7EC, "Enemy::~Enemy", Enemy::destruct);

void Enemy::destructDelete(Enemy *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5CC878, "Enemy::~Enemy (deleting)", Enemy::destructDelete);

// The constructor's motion-end command (CommandParser slot at +960): ActFlag 5.
void Enemy::motionEndLambda(uint32_t lambda) {
    bt::GuestFrame f(8);
    rw::save<uint32_t>(f.addr(0), 5);
    rw::call(g::dequeActFlag_pushBack, {actor::ld(lambda + 4) + 1248, f.addr(0)});
}

RW_FUNCTION(0x5CC884, "Enemy::Enemy $_0 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5CC88C, "Enemy::Enemy $_0 __func::__clone()", (fnthunk::cloneWords<g::motionEndLambda, 8, 1>));
RW_FUNCTION(0x5CC8B8, "Enemy::Enemy $_0 __func::__clone(__base*)", fnthunk::cloneInto1<g::motionEndLambda>);
RW_FUNCTION(0x5CC8D8, "Enemy::Enemy $_0 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5CC8DA, "Enemy::Enemy $_0 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5CC8E4, "Enemy::Enemy $_0 (motion end: ActFlag 5)", Enemy::motionEndLambda);
RW_FUNCTION(0x5CC930, "Enemy::Enemy $_0 __func::target", fnthunk::target<0xE4A960>);
RW_FUNCTION(0x5CC950, "Enemy::Enemy $_0 __func::target_type", fnthunk::targetType<0xF8FB40>);
