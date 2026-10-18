// CarouselDeck: the deck rolling to the next medal, and a medal's attack (with the special attack cut-in).
#include "CarouselDeck.hpp"

using namespace cdeck;

namespace {
constexpr uint32_t VT_LAMBDA_ROLL_DONE = 0xF5D280;   // advanceDeck $_?: the roll animation finished
constexpr uint32_t CarouselDeck_playGuiltBonusEffect = 0x5D129D;   // (self, shared_ptr<const master::Medal>, guilt)
constexpr uint32_t CUTIN_LIGHT = 1, CUTIN_DARK = 2;
}  // namespace

// The deck rolls on to the next medal (back to the first after the last): the info and burst hints of the
// current medal go, a roll still running is finished at once.
void CarouselDeck::advanceDeck(CarouselDeck *self) {
    setVisible(self->friendMedalUi, 0);
    vcall(self->keybladeInfo, node::removeAllChildren, {self->keybladeInfo});
    setPossibleBurst(self, 0, 0);
    removeArmature(self, self->longTapArmature);
    onCenterButton(self, 0);
    if (self->rolling) finishRollAnimation(self);
    const int32_t next = self->currentIndex + 1;
    self->currentIndex = self->deckSize > next ? next : 0;
    self->rolling = 1;
    Frame f;
    const uint32_t done = newFunction(f, VT_LAMBDA_ROLL_DONE, {rw::guest_addr(self)});
    call(ActionObject_playCallFunc, {self->rollAction, call(CallFunc_create, {done})});
    functionDestroy(done);
}
RW_FUNCTION(0x5D0C90, "CarouselDeck::advanceDeck", CarouselDeck::advanceDeck);

// advanceDeck's roll-finished lambda: the new medal takes the attack place, the special attack offer is shown
void rollDone(uint32_t functor) {
    CarouselDeck *self = const_cast<CarouselDeck *>(rw::host_ptr<CarouselDeck>(ld32(functor + 4)));
    CarouselDeck::finishRollAnimation(self);
    CarouselDeck::setPossibleBurst(self, 1, 1);
}
RW_FUNCTION(0x5D2EFC, "CarouselDeck::advanceDeck $_? operator() (roll done)", rollDone);

// init $_4: the normal attack animation's movement event - on complete / loop complete the attack ends
void attackMovementEvent(uint32_t functor, uint32_t, uint32_t typePtr) {
    if (ld32(typePtr) - 1 > 1) return;
    CarouselDeck::finishActionAttack(const_cast<CarouselDeck *>(rw::host_ptr<CarouselDeck>(ld32(functor + 4))));
}
RW_FUNCTION(0x5D30FA, "CarouselDeck::init $_4 operator() (attack movement event)", attackMovementEvent);

// The attacking medal plays its attack: any attack or cut-in still showing is cleared first; for a special
// attack (burst) the medal's light / dark cut-in (its guilt variant when the medal has guilt) plays with the
// medal's cut-in picture, the guilt bonus effect and the guilt shader.
void CarouselDeck::actionAttack(CarouselDeck *self, uint32_t burst) {
    if (!self->attackingMedal) return;
    if (removeArmature(self, self->medalAttackArmature)) finishActionAttack(self);
    for (uint32_t k = 0; k < 4; k++)
        if (removeArmature(self, self->cutinArmatures[k])) finishActionBurst(self, self->cutinArmatures[k]);
    if (vcall(self->attackingMedal, node::getParent, {self->attackingMedal}))
        vcall(self->attackingMedal, node::removeFromParent, {self->attackingMedal});
    vcall(self->medalDummy, node::removeAllChildren, {self->medalDummy});
    self->bursting = 0;

    if (burst == 1) {
        Frame f;
        const uint32_t medal = f.alloc(8);   // shared_ptr<const master::Medal>
        call(Medal_create, {medal, uint32_t(self->cutinMedalIds[self->currentIndex])});
        if (const uint32_t data = ld32(medal)) {
            if (const uint32_t sprite = self->cutinSprite) {
                const uint32_t setTexture = ld32(ld32(sprite) + VT_SPRITE_SET_TEXTURE);
                const uint32_t image = f.alloc(4);
                call(SafeBuffer_read, {data, image, ld32(data + 0xC) + MEDAL_IMAGE, 4, MEDAL_IMAGE});
                const uint32_t path = f.alloc(0x404), s = f.alloc(12);
                call(0x49140D, {path, STR_FMT_CUTIN, ld32(image)});   // StringBuffer(fmt, image id)
                strInit(s, path, host_strlen(path));
                call(setTexture, {sprite, s});
                strFree(s);
                if (vcall(self->cutinSprite, node::getParent, {self->cutinSprite}))
                    vcall(self->cutinSprite, node::removeFromParent, {self->cutinSprite});
            }
            const uint32_t guilt = self->guiltValues[self->currentIndex];
            const uint32_t cutinType = f.alloc(4);
            call(SafeBuffer_read, {data, cutinType, ld32(data + 0xC) + MEDAL_CUTIN_TYPE, 4, MEDAL_CUTIN_TYPE});
            uint32_t variant = guilt ? 1 : 0;
            bool known = true;
            if (ld32(cutinType) == CUTIN_DARK) variant |= 2;
            else if (ld32(cutinType) != CUTIN_LIGHT) known = false;
            const uint32_t armature = known ? uint32_t(self->cutinArmatures[variant]) : 0;
            if (armature) {
                vcall(rw::guest_addr(self), node::addChildZ, {rw::guest_addr(self), armature, 0x14});
                const uint32_t anim = vcall(armature, VT_GET_ANIMATION, {armature});
                vcall(anim, VT_ANIM_GOTO_AND_PLAY, {anim, 0});
                self->bursting = 1;
                const uint32_t holder = vcall(armature, VT_GET_USER_OBJECT, {armature});
                vcall(holder, node::addChildZ, {holder, self->cutinSprite, 1});
                if (guilt) {
                    const uint32_t copy = f.alloc(8);
                    st32(copy, data);
                    const uint32_t ctrl = ld32(medal + 4);
                    st32(copy + 4, ctrl);
                    if (ctrl) guest::addRef(ctrl);
                    call(CarouselDeck_playGuiltBonusEffect,
                         {rw::guest_addr(self), copy, self->guiltValues[self->currentIndex]});
                    if (ctrl) guest::release(ctrl);
                    if (const uint32_t shader = self->guiltShader) {
                        const uint32_t icon = f.alloc(4);
                        call(SafeBuffer_read, {data, icon, ld32(data + 0xC) + MEDAL_GUILT_ICON, 4, MEDAL_GUILT_ICON});
                        call(GuiltBurstCutInShader_setShaderStart, {shader, ld32(icon)});
                    }
                }
            }
        }
        sharedRelease(medal);
    }

    // the medal moves onto the attack place and the normal attack animation plays
    setVisible(self->abilityInfos[0], 0);
    vcall(self->medalDummy, node::addChild, {self->medalDummy, self->attackingMedal});
    self->attackingMedal = 0;
    vcall(rw::guest_addr(self), node::addChildZ, {rw::guest_addr(self), self->medalAttackArmature, 10});
    const uint32_t anim = vcall(self->medalAttackArmature, VT_GET_ANIMATION, {self->medalAttackArmature});
    vcall(anim, VT_ANIM_GOTO_AND_PLAY, {anim, 0});
    self->attacking = 1;
    setVisible(self->burstReadyArmature, 0);
}
RW_FUNCTION(0x5D0E94, "CarouselDeck::actionAttack", CarouselDeck::actionAttack);
