// BaseActorManager: the base of the stage / raid actor managers (cocos2d::Ref): the list of avatar actors
// (cocos2d::Vector<AvatarActor*>), the player's and the friend's avatar, the medal deck, whether the special
// attack (burst) is possible with the current avatar style and the play style (2 = raid).
// StageActorManager (battle/StageActorManager.hpp) derives from it.
#pragma once

#include "../scene/SceneCommon.hpp"

struct BaseActorManager {
    rw::Field<uint32_t> vtable;            // +0   cocos2d::Ref
    const uint8_t refBody[16];             // +4
    rw::Field<uint32_t> avatars[3];        // +20  cocos2d::Vector<AvatarActor*> (std::vector: begin, end, cap)
    rw::Field<uint32_t> player;            // +32  AvatarActor* the player
    rw::Field<uint32_t> friendAvatar;      // +36  AvatarActor* the friend, may be 0
    rw::Field<uint32_t> medalDeck;         // +40  MedalDeck*
    rw::Field<uint8_t> burstEnable;        // +44  update_burst_enable
    const uint8_t pad_2D[3];
    rw::Field<int32_t> playStyle;          // +48  StageManager::PlayStyle (2 = raid)

    static constexpr uint32_t VTABLE = 0xFA8298;   // _ZTV16BaseActorManager + 8

    static BaseActorManager *construct(BaseActorManager *self);
    static BaseActorManager *destruct(BaseActorManager *self);
    static void destructDelete(BaseActorManager *self);
    static bool init(BaseActorManager *self);
    static void setupAvatarActorPlayable(BaseActorManager *self, uint32_t style);
    static void setupAvatarActorFriend(BaseActorManager *self, uint32_t style);
    static BaseActorManager *update_burst_enable(BaseActorManager *self);
    static bool isPlayerAttackSkipStatus(BaseActorManager *self, uint32_t force);
};
RW_LAYOUT(BaseActorManager, avatars, 20);
RW_LAYOUT(BaseActorManager, player, 32);
RW_LAYOUT(BaseActorManager, burstEnable, 44);
RW_LAYOUT(BaseActorManager, playStyle, 48);
