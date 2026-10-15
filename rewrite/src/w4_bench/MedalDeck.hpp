// MedalDeck: the battle's medal deck (a cocos2d::Node). The player's deck holds up to 5 medals plus an optional
// friend (support) medal; each slot is a MedalInfo: the master::Medal row and a "Property" SafeBuffer with the
// per-battle values (attack / defense from BattleUtil::calcMedalStatus, the supporting user, the burst flags).
// During the player's turn (state 1) the medals are used one after the other (currentIndex); when all were used
// the deck switches to the enemy's turn (state 2). Three std::function callbacks report those transitions.
#pragma once

#include "../core/Guest.hpp"
#include "../core/SafeBuffer.hpp"

// libc++ std::function (32-bit): a 16-byte inline buffer, then the callable (__base*) pointer; the member is
// 8-byte aligned, so 24 bytes in a struct.
struct MedalDeckFunction {
    const uint8_t buffer[16];             // +0  inline storage of a small callable
    rw::Field<uint32_t> impl;             // +16 __base*: == this (inline), heap pointer, or 0 (empty)
    const uint8_t pad_14[4];
};
RW_SIZE(MedalDeckFunction, 24);

// MedalDeck::MedalInfo (40 bytes)
struct MedalInfo {
    guest::SharedPtr medal;               // +0  std::shared_ptr<const master::Medal> (a SafeBuffer row)
    SafeBuffer property;                  // +8  MedalDeck::Property (vtable PROPERTY_VTABLE)

    // data positions inside `property`
    static constexpr uint32_t ATTACK = 0;               // int, written by the calcMedalStatus callback
    static constexpr uint32_t DEFENSE = 4;              // int, written by the calcMedalStatus callback
    static constexpr uint32_t SUPPORT_USER = 8;         // uint64 user id of the friend lending the medal (0 = own)
    static constexpr uint32_t BURST_LEVEL = 16;         // from UserMedal::Medal + 28
    static constexpr uint32_t GUILT = 20;               // from UserMedal::Medal + 40
    static constexpr uint32_t BURST_SINGLE_SELECT = 24; // 1 byte: isBurstSingleSelect(burst id)
};
RW_LAYOUT(MedalInfo, property, 8);
RW_SIZE(MedalInfo, 40);

struct MedalDeck {
    const uint8_t node[0x270];            // +0     cocos2d::Node base (vtable at +0)
    MedalInfo medals[6];                  // +0x270 5 deck medals + the friend medal (after the deck medals)
    rw::Field<int32_t> deckCount;         // +0x360 deck medals (at most 5)
    rw::Field<int32_t> friendCount;       // +0x364 0 or 1
    rw::Field<int32_t> state;             // +0x368 1 = player's turn (medals in use), 2 = enemy's turn
    rw::Field<int32_t> turnCount;         // +0x36C player turns completed
    rw::Field<int32_t> currentIndex;      // +0x370 medal in use during state 1
    const uint8_t pad_374[4];
    MedalDeckFunction onAdvance;          // +0x378 std::function<void()>: called on every medal advance (state 1)
    MedalDeckFunction onDeckEnd;          // +0x390 std::function<void()>: all medals used, state 1 -> 2
    MedalDeckFunction onTurnStart;        // +0x3A8 std::function<void()>: state 2 -> 1, also turnEnd()

    static constexpr uint32_t VTABLE = 0xF934B0;           // _ZTV9MedalDeck + 8
    static constexpr uint32_t PROPERTY_VTABLE = 0xF93748;  // _ZTVN9MedalDeck8PropertyE + 8
    static constexpr uint32_t ADD_ONTO_DECK_LAMBDA_VTABLE = 0xF5DAA4;  // addOntoDeck $_0 __func vtable + 8

    static constexpr int32_t STATE_PLAYER = 1;
    static constexpr int32_t STATE_ENEMY = 2;

    // Burst buffer kinds of getBitFieldBurstAddStatus (which block of the burst row is summarised)
    static constexpr uint32_t BURST_BUFFER_1 = 1, BURST_BUFFER_2 = 2, BURST_BUFFER_3 = 3;

    // master::Medal row positions
    static constexpr uint32_t MEDAL_HAS_BURST = 0x4A8;     // int: 1 = the medal has a usable burst
    static constexpr uint32_t MEDAL_BURST_ID = 0x4AC;      // int: master::Burst id
    // master::Burst row positions
    static constexpr uint32_t BURST_GAUGE = 0x168;         // int: gauge cost
    static constexpr uint32_t BURST_SINGLE_SELECT = 0x160; // int: 1 = single target

    static MedalDeck *construct(MedalDeck *self);
    static MedalDeck *destruct(MedalDeck *self);
    static uint32_t create();

    static uint32_t addMedalOntoDeck(MedalDeck *self, uint32_t keyblade, uint32_t medalIds, int32_t count,
                                     uint32_t onAdded);
    static uint32_t addOntoDeck(MedalDeck *self, uint32_t userMedal, MedalInfo *info, uint32_t onAdded,
                                uint32_t keyblade, int32_t index);
    static uint32_t addFriendMedalOntoDeck(MedalDeck *self, uint32_t keyblade, uint32_t userMedal, uint32_t onAdded);

    static int32_t advanceState(MedalDeck *self);
    static MedalInfo *getCurrentMedalInfo(MedalDeck *self);
    static int32_t getCurrentAttack(MedalDeck *self);
    static int32_t getCurrentDefense(MedalDeck *self);
    static uint32_t forEachMedal(MedalDeck *self, uint32_t fn);
    static uint32_t isBurstSingleSelect(uint32_t self, int32_t burstId);
    static uint32_t isNextEnemyTurn(MedalDeck *self);
    static uint32_t isCurrentFirstMedal(MedalDeck *self);
    static uint32_t isCurrentFriendMedal(MedalDeck *self);
    static uint32_t isCurrentBurstSingleSelect(MedalDeck *self);
    static uint32_t isValidCurrentBurst(MedalDeck *self);
    static int32_t getCurrentBurstGauge(MedalDeck *self);
    static uint32_t getBitFieldBurstAddStatus(uint32_t self, uint32_t type, int32_t burstId);

    // addOntoDeck's $_0 (std::function<void(int, int)>): stores attack and defense into the Property buffer
    static void addOntoDeckLambda(uint32_t func, const int32_t *attack, const int32_t *defense);
};
RW_LAYOUT(MedalDeck, medals, 0x270);
RW_LAYOUT(MedalDeck, deckCount, 0x360);
RW_LAYOUT(MedalDeck, currentIndex, 0x370);
RW_LAYOUT(MedalDeck, onAdvance, 0x378);
RW_LAYOUT(MedalDeck, onDeckEnd, 0x390);
RW_LAYOUT(MedalDeck, onTurnStart, 0x3A8);
RW_SIZE(MedalDeck, 0x3C0);
