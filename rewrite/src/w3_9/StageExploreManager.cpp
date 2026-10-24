#include "StageExploreManager.hpp"

#include "../scene_main/FunctionThunks.hpp"

namespace {
namespace g {
constexpr uint32_t ref_ctor = rwlib::cocos2d::Ref::ctor_base;               // cocos2d::Ref::Ref()
constexpr uint32_t vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;              // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t vec2_between = rwlib::cocos2d::Vec2::ctor_base__RKS0_S2_;           // cocos2d::Vec2::Vec2(const Vec2& from, const Vec2& to) = to - from
constexpr uint32_t vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;              // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;              // cocos2d::Vec2::~Vec2()
constexpr uint32_t vec2_add = rwlib::cocos2d::Vec2::add;               // cocos2d::Vec2::add(const Vec2&)
constexpr uint32_t vec2_dot = rwlib::cocos2d::Vec2::dot;               // cocos2d::Vec2::dot(const Vec2&, const Vec2&) (static)
constexpr uint32_t vec2_length = rwlib::cocos2d::Vec2::length;            // cocos2d::Vec2::length() const
constexpr uint32_t vec2_normalize = rwlib::cocos2d::Vec2::normalize;         // cocos2d::Vec2::normalize()
constexpr uint32_t vec2_scale = rwlib::cocos2d::Vec2::scale__f;             // cocos2d::Vec2::scale(float)
constexpr uint32_t vec2_ZERO = 0x1006950;             // cocos2d::Vec2::ZERO
constexpr uint32_t roundf_ = 0x419C54;                // roundf (PLT)
constexpr uint32_t avatar_changeMotion = 0x5C63C5;    // Avatar::changeMotion(Avatar::Motion)
constexpr uint32_t actor_setSafeMovePosition = 0x7415A5;  // AvatarActor::setSefeMoveMapPotision(Vec2&)
constexpr uint32_t field_isCollisionDetection = 0x7569ED;  // StageFieldManager::isCollisionDetection(const Vec2&, const Vec2&)
}  // namespace g

// AvatarActor fields
constexpr uint32_t ACTOR_POSITION = 624;   // cocos2d::Vec2 map position (MapNode)
constexpr uint32_t ACTOR_AVATAR = 648;     // Avatar*
// cocos2d::Node virtual slots
constexpr uint32_t VSLOT_SET_SCALE_X = 10;
constexpr uint32_t VSLOT_GET_SCALE_X = 11;
constexpr uint32_t MOTION_DODGE_ROLL = 39;  // Avatar::Motion
constexpr uint32_t ROLL_FRAMES = 29;        // entries of the roll curve
constexpr uint32_t ROLL_FRAME_TIME = 0x3D088889;  // float 1/30: one roll frame (s)

float roundF(float v) { return rw::callf(g::roundf_, {rw::fbits(v)}); }
float frameFloat(uint32_t a) { return rw::bitsf(rw::load<uint32_t>(a)); }
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
StageExploreManager *StageExploreManager::create() {
    auto *self = rw::host_ptr<StageExploreManager>(rw::call(bt::fn::operator_new, {0x40}));
    construct(self);
    if (rw::vcall(self, 2, {rw::arg(self)}) == 1) {          // init()
        rw::call(bt::fn::ref_autorelease, {rw::arg(self)});   // cocos2d::Ref::autorelease
        return self;
    }
    rw::vcall(self, 1, {rw::arg(self)});                       // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x5FC87E, "StageExploreManager::create", StageExploreManager::create);

StageExploreManager *StageExploreManager::construct(StageExploreManager *self) {
    rw::call(g::ref_ctor, {rw::arg(self)});
    self->vtable = VTABLE;
    self->rolling = 0;
    rw::call(g::vec2_copy, {rw::arg(&self->rollDirection), g::vec2_ZERO});
    rw::set<uint32_t>(self->callback, 16, 0);                  // empty std::function
    self->rollFrame = 0;
    rw::set<uint32_t>(&self->rollTime, 0, 0);
    return self;
}
RW_FUNCTION(0x607CE0, "StageExploreManager::StageExploreManager", StageExploreManager::construct);

StageExploreManager *StageExploreManager::destruct(StageExploreManager *self) {
    self->vtable = VTABLE;
    thunk::destroyFunction(rw::arg(self->callback));          // ~std::function
    rw::call(g::vec2_dtor, {rw::arg(&self->rollDirection)});
    rw::call(bt::fn::ref_dtor, {rw::arg(self)});
    return self;
}
RW_FUNCTION(0x7546AC, "StageExploreManager::~StageExploreManager", StageExploreManager::destruct);

void StageExploreManager::destructDelete(StageExploreManager *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {rw::arg(self)});
}
RW_FUNCTION(0x7546FC, "StageExploreManager::~StageExploreManager(deleting)", StageExploreManager::destructDelete);

uint32_t StageExploreManager::init(StageExploreManager *) { return 1; }
RW_FUNCTION(0x754708, "StageExploreManager::init", StageExploreManager::init);

// ------------------------------------------------------------------------------------------- dodge roll
void StageExploreManager::startDodgeRoll(StageExploreManager *self, const bt::Vec2 *direction) {
    if (self->rolling != 0) return;
    self->rolling = 1;
    self->rollDirection.y = direction->y;
    self->rollDirection.x = direction->x;
    self->rollFrame = 0;
    rw::set<uint32_t>(&self->rollTime, 0, 0);
}
RW_FUNCTION(0x75470C, "StageExploreManager::startDodgeRoll", StageExploreManager::startDodgeRoll);

void StageExploreManager::stopDodgeRoll(StageExploreManager *self) {
    self->rolling = 0;
    rw::set<uint32_t>(&self->rollDirection, 4, rw::load<uint32_t>(g::vec2_ZERO + 4));
    rw::set<uint32_t>(&self->rollDirection, 0, rw::load<uint32_t>(g::vec2_ZERO));
    self->rollFrame = 0;
    rw::set<uint32_t>(&self->rollTime, 0, 0);
}
RW_FUNCTION(0x754728, "StageExploreManager::stopDodgeRoll", StageExploreManager::stopDodgeRoll);

// One update of a running roll: at its start the avatar plays the roll motion and turns to face the roll; then the
// actor moves along the curve for every frame index up to the elapsed time (at most 28); after the last frame
// the roll ends. Returns 1 while rolling (0 when no roll runs).
uint32_t StageExploreManager::actionDodgeRoll(StageExploreManager *self, float dt, uint32_t avatarActor) {
    if (self->rolling != 1) return 0;
    if (self->rollTime.get() <= 0.0f) {
        rw::call(g::avatar_changeMotion, {rw::load<uint32_t>(avatarActor + ACTOR_AVATAR), MOTION_DODGE_ROLL});
        float dirX = self->rollDirection.x;
        if (!(dirX == 0.0f)) {
            bool left = dirX < 0.0f;
            const void *actor = rw::host_ptr<uint32_t>(avatarActor);
            float scaleX = rw::bitsf(rw::vcall(actor, VSLOT_GET_SCALE_X, {avatarActor}));
            float facing = left ? 1.0f : -1.0f;
            if (!(scaleX == facing)) rw::vcall(actor, VSLOT_SET_SCALE_X, {avatarActor, rw::fbits(facing)});
        }
    }
    float time = self->rollTime;
    bt::GuestFrame frame(8);
    uint32_t pos = frame.addr();                               // Vec2 pos(actor position)
    rw::call(g::vec2_copy, {pos, avatarActor + ACTOR_POSITION});
    int32_t reached = bt::f2iz(time / rw::bitsf(ROLL_FRAME_TIME));
    uint32_t last = uint32_t(reached) > 28u ? 28u : uint32_t(reached);
    int32_t next = self->rollFrame;
    if (next <= int32_t(last)) {
        int32_t k = int32_t(uint32_t(next) - 1u);
        for (;;) {
            float step = rw::bitsf(rw::load<uint32_t>(ROLL_CURVE + 4u * uint32_t(next)));
            rw::save<uint32_t>(pos, rw::fbits(frameFloat(pos) + step * self->rollDirection.x.get()));
            rw::save<uint32_t>(pos + 4, rw::fbits(frameFloat(pos + 4) + (step * self->rollDirection.y.get()) * 0.5f));
            self->rollFrame = int32_t(uint32_t(next) + 1u);
            rw::call(g::actor_setSafeMovePosition, {avatarActor, pos});
            k++;
            if (!(k < int32_t(last))) break;
            next = self->rollFrame;
        }
        next = self->rollFrame;
    }
    self->rollTime = self->rollTime.get() + dt;
    if (uint32_t(next) >= ROLL_FRAMES) {
        self->rolling = 0;
        rw::set<uint32_t>(&self->rollDirection, 4, rw::load<uint32_t>(g::vec2_ZERO + 4));
        rw::set<uint32_t>(&self->rollDirection, 0, rw::load<uint32_t>(g::vec2_ZERO));
        self->rollFrame = 0;
        rw::set<uint32_t>(&self->rollTime, 0, 0);
    }
    rw::call(g::vec2_dtor, {pos});
    return 1;
}
RW_FUNCTION(0x75474C, "StageExploreManager::actionDodgeRoll", StageExploreManager::actionDodgeRoll);

// ------------------------------------------------------------------------------------------- movement
// Walk from the actor's (rounded) position toward the (rounded) target one unit at a time while the field allows
// it. Returns 1 when the actor moved at least one step.
uint32_t StageExploreManager::moveAsMuchAsPossible(StageExploreManager *, uint32_t avatarActor, bt::Vec2 *target) {
    bt::GuestFrame frame(24);
    uint32_t step = frame.addr(0), to = frame.addr(8), pos = frame.addr(16);
    float px = roundF(rw::bitsf(rw::load<uint32_t>(avatarActor + ACTOR_POSITION)));
    float py = roundF(rw::bitsf(rw::load<uint32_t>(avatarActor + ACTOR_POSITION + 4)));
    rw::call(g::vec2_ctor, {pos, rw::fbits(px), rw::fbits(py)});
    float tx = roundF(target->x);
    float ty = roundF(target->y);
    rw::call(g::vec2_ctor, {to, rw::fbits(tx), rw::fbits(ty)});
    rw::call(g::vec2_between, {step, pos, to});
    int32_t steps = bt::f2iz(roundF(rw::callf(g::vec2_length, {step})));
    rw::call(g::vec2_normalize, {step});
    uint32_t moved = 0;
    if (steps > 0) {
        int32_t k = 0;
        do {
            rw::call(g::vec2_add, {pos, step});
            k++;
            if (rw::call(g::actor_setSafeMovePosition, {avatarActor, pos}) == 0) break;
            moved = 1;
        } while (k < steps);
    }
    rw::call(g::vec2_dtor, {step});
    rw::call(g::vec2_dtor, {to});
    rw::call(g::vec2_dtor, {pos});
    return moved & 1;
}
RW_FUNCTION(0x75494C, "StageExploreManager::moveAsMuchAsPossible", StageExploreManager::moveAsMuchAsPossible);

// Move toward the target; then probe the 8 neighbouring directions (each scaled to the full distance) and jump to
// the free candidate whose direction is closest to the wanted one (largest positive dot product), moving on
// toward the target from there. Returns 1 when such a candidate was found.
uint32_t StageExploreManager::searchMovablePosition(StageExploreManager *self, uint32_t avatarActor,
                                                    bt::Vec2 *target, uint32_t fieldManager) {
    moveAsMuchAsPossible(self, avatarActor, target);          // (the original passes a stale `this`; unused)
    bt::GuestFrame frame(56);
    uint32_t cand = frame.addr(0), probe = frame.addr(8), dir = frame.addr(16), wanted = frame.addr(24),
             best = frame.addr(32), to = frame.addr(40), pos = frame.addr(48);
    float px = roundF(rw::bitsf(rw::load<uint32_t>(avatarActor + ACTOR_POSITION)));
    float py = roundF(rw::bitsf(rw::load<uint32_t>(avatarActor + ACTOR_POSITION + 4)));
    rw::call(g::vec2_ctor, {pos, rw::fbits(px), rw::fbits(py)});
    uint32_t actorPos = avatarActor + ACTOR_POSITION;
    float tx = roundF(target->x);
    float ty = roundF(target->y);
    rw::call(g::vec2_ctor, {to, rw::fbits(tx), rw::fbits(ty)});
    rw::call(g::vec2_copy, {best, g::vec2_ZERO});
    rw::call(g::vec2_between, {wanted, pos, to});
    uint32_t distance = rw::call(g::vec2_length, {wanted});
    int32_t ix = bt::f2iz(frameFloat(pos));
    int32_t iy = bt::f2iz(frameFloat(pos + 4));
    uint32_t found = 0;
    float bestDot = 0.0f;
    for (uint32_t k = 0; k < 8; k++) {
        int32_t ox = rw::load<int32_t>(NEIGHBOURS + 8 * k);
        int32_t oy = rw::load<int32_t>(NEIGHBOURS + 8 * k + 4);
        float cx = float(int32_t(uint32_t(ox) + uint32_t(ix)));
        float cy = float(int32_t(uint32_t(oy) + uint32_t(iy)));
        rw::call(g::vec2_ctor, {probe, rw::fbits(cx), rw::fbits(cy)});
        rw::call(g::vec2_between, {dir, pos, probe});
        rw::call(g::vec2_dtor, {probe});
        rw::call(g::vec2_normalize, {dir});
        rw::call(g::vec2_scale, {dir, distance});
        rw::call(g::vec2_copy, {cand, pos});
        rw::call(g::vec2_add, {cand, dir});
        if (rw::call(g::field_isCollisionDetection, {fieldManager, actorPos, cand}) == 0) {
            float dot = rw::callf(g::vec2_dot, {dir, wanted});
            if (dot > bestDot) {
                rw::save<uint32_t>(best, rw::load<uint32_t>(cand));
                rw::save<uint32_t>(best + 4, rw::load<uint32_t>(cand + 4));
                found = 1;
                bestDot = dot;
            }
        }
        rw::call(g::vec2_dtor, {cand});
        rw::call(g::vec2_dtor, {dir});
    }
    if (found & 1) {
        uint32_t r = rw::call(g::actor_setSafeMovePosition, {avatarActor, best});
        moveAsMuchAsPossible(rw::host_ptr<StageExploreManager>(r), avatarActor, target);  // (stale `this` again)
    }
    rw::call(g::vec2_dtor, {wanted});
    rw::call(g::vec2_dtor, {best});
    rw::call(g::vec2_dtor, {to});
    rw::call(g::vec2_dtor, {pos});
    return found & 1;
}
RW_FUNCTION(0x754A64, "StageExploreManager::searchMovablePosition", StageExploreManager::searchMovablePosition);
