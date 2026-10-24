// BaseActorManager: the avatar list and the player / friend avatars of the actor managers (see the header).
#include "BaseActorManager.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Ref_ctor = rwlib::cocos2d::Ref::ctor_base, Ref_dtor = rwlib::cocos2d::Ref::dtor_base;
constexpr uint32_t AvatarVector_clear = 0x485BA3;      // cocos2d::Vector<AvatarActor*>::clear
constexpr uint32_t AvatarVector_dtor = 0x485A63;       // cocos2d::Vector<AvatarActor*>::~Vector
constexpr uint32_t AvatarVector_pushBack = 0x6425D5;   // cocos2d::Vector<AvatarActor*>::pushBack
constexpr uint32_t AvatarActor_create = 0x73FD21;      // AvatarActor::create(const Avatar::Style*)
constexpr uint32_t Avatar_preloadResources = 0x5C7ED1;
constexpr uint32_t Badstatus_create = 0x779525;        // master::Badstatus::create(sret, id)
constexpr uint32_t Random_getInstance = 0x492055;
constexpr uint32_t Random_generateUniformInt = 0x492225;   // (random, min, max, uint* state)

constexpr uint32_t ACTOR_AVATAR = 648;                 // AvatarActor: Avatar*
constexpr uint32_t ACTOR_PLAYABLE = 660;               // AvatarActor: byte, the player's own avatar
constexpr uint32_t AVATAR_STYLE_KIND = 0x524;          // Avatar: the style's kind (11, 16, 18: no special attack)
constexpr uint32_t APPSTATE_STAGE_STATE = 32;          // AppState: StageState*
constexpr uint32_t STAGE_STATE_STATUS = 32;            // StageState: IStatusProtocol base (slot 0 getCondition)
constexpr uint32_t BADSTATUS_SKIP = 121;               // master::Badstatus row used for the attack skip
constexpr uint32_t BADSTATUS_RATE = 0x54;              // its rate (out of 10000)
constexpr int32_t PLAY_STYLE_RAID = 2;

// The StageState condition `type` (IStatusProtocol::getCondition, sret 12 bytes at out): its third word
uint32_t conditionTurns(uint32_t out, uint32_t type) {
    uint32_t state = ld32(call(fn::AppState_getInstance, {}) + APPSTATE_STAGE_STATE);
    uint32_t status = state + STAGE_STATE_STATUS;
    call(ld32(ld32(status)), {out, status, type});
    return ld32(out + 8);
}
}  // namespace

BaseActorManager *BaseActorManager::construct(BaseActorManager *self) {
    call(Ref_ctor, {rw::guest_addr(self)});
    self->vtable = VTABLE;
    self->playStyle = 0;
    // memset(+20, 0, 24): the avatar list, player, friend, medal deck
    for (auto &w : self->avatars) w = 0;
    self->player = 0;
    self->friendAvatar = 0;
    self->medalDeck = 0;
    return self;
}
RW_FUNCTION(0x74E720, "BaseActorManager::BaseActorManager", BaseActorManager::construct);

BaseActorManager *BaseActorManager::destruct(BaseActorManager *self) {
    self->vtable = VTABLE;
    uint32_t list = rw::guest_addr(&self->avatars);
    call(AvatarVector_clear, {list});
    call(AvatarVector_dtor, {list});
    call(Ref_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x74E758, "BaseActorManager::~BaseActorManager", BaseActorManager::destruct);

void BaseActorManager::destructDelete(BaseActorManager *self) {
    call(fn::operator_delete, {rw::guest_addr(destruct(self))});
}
RW_FUNCTION(0x74E7B4, "BaseActorManager::~BaseActorManager (deleting)", BaseActorManager::destructDelete);

bool BaseActorManager::init(BaseActorManager *) { return true; }
RW_FUNCTION(0x74E7C0, "BaseActorManager::init", BaseActorManager::init);

// The player's avatar actor (tag 0, flagged playable, resources preloaded, listed, shown).
void BaseActorManager::setupAvatarActorPlayable(BaseActorManager *self, uint32_t style) {
    uint32_t actor = call(AvatarActor_create, {style});
    vcall(actor, node::setTag, {actor, 0});
    st8(actor + ACTOR_PLAYABLE, 1);
    call(Avatar_preloadResources, {ld32(actor + ACTOR_AVATAR)});
    call(AvatarVector_pushBack, {rw::guest_addr(&self->avatars), actor});
    self->player = actor;
    vcall(actor, node::setVisible, {actor, 1});
}
RW_FUNCTION(0x74E7C4, "BaseActorManager::setupAvatarActorPlayable", BaseActorManager::setupAvatarActorPlayable);

// The friend's avatar actor (tag 1, listed, hidden); none without a style.
void BaseActorManager::setupAvatarActorFriend(BaseActorManager *self, uint32_t style) {
    if (!style) {
        self->friendAvatar = 0;
        return;
    }
    uint32_t actor = call(AvatarActor_create, {style});
    vcall(actor, node::setTag, {actor, 1});
    call(Avatar_preloadResources, {ld32(actor + ACTOR_AVATAR)});
    call(AvatarVector_pushBack, {rw::guest_addr(&self->avatars), actor});
    self->friendAvatar = actor;
    vcall(actor, node::setVisible, {actor, 0});
}
RW_FUNCTION(0x74E81A, "BaseActorManager::setupAvatarActorFriend", BaseActorManager::setupAvatarActorFriend);

// The special attack is possible unless the player's style kind is 11 (in raids also 16 and 18).
BaseActorManager *BaseActorManager::update_burst_enable(BaseActorManager *self) {
    int32_t kind = int32_t(ld32(ld32(self->player + ACTOR_AVATAR) + AVATAR_STYLE_KIND));
    bool enable;
    if (self->playStyle == PLAY_STYLE_RAID) enable = kind != 11 && kind != 16 && kind != 18;
    else enable = kind != 11;
    self->burstEnable = enable ? 1 : 0;
    return self;
}
RW_FUNCTION(0x74E870, "BaseActorManager::update_burst_enable", BaseActorManager::update_burst_enable);

// Whether the player's attack goes through: with condition 1 active (and not forced) yes; otherwise, with
// condition 2 active, a roll of 1..10000 against the rate of bad status 121; no condition 2: no.
bool BaseActorManager::isPlayerAttackSkipStatus(BaseActorManager *, uint32_t force) {
    Frame f;
    uint32_t out = f.alloc(12);
    if (conditionTurns(out, 1) != 0 && force != 1) return true;
    if (conditionTurns(out, 2) == 0) return false;
    uint32_t row = masterRow(f, Badstatus_create, BADSTATUS_SKIP);
    int32_t rate = safe32(ld32(row), BADSTATUS_RATE);
    int32_t roll = int32_t(call(Random_generateUniformInt, {call(Random_getInstance, {}), 1, 10000, 0}));
    sharedRelease(row);
    return rate >= roll;
}
RW_FUNCTION(0x74E8AC, "BaseActorManager::isPlayerAttackSkipStatus", BaseActorManager::isPlayerAttackSkipStatus);
