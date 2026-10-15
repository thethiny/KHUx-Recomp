// SceneBattleResult: the result screen after a quest (stage, raid or colosseum): dialogs one after the other
// (rewards, clear rank, new keyblades, level up, new Lux rank, user title). A cocos2d::Layer subclass.
#pragma once

#include "../core/Guest.hpp"
#include "rw/rw.hpp"

struct SceneBattleResultLayout {
    rw::Field<uint32_t> vtable;   // +0x000
    const uint8_t pad_004[0x20C];
    const uint8_t onEnterCallback[20];   // std::function<void()> (callable pointer at +0x220)
    const uint8_t pad_224[0x4];
    const uint8_t onExitCallback[20];   // std::function<void()> (callable pointer at +0x238)
    const uint8_t pad_23C[0x4C];
    rw::Field<uint32_t> resultKind;   // +0x288 0 stage, 1 raid, 2 colosseum (3 and the raid boss intro share the raid scene)
    guest::SharedPtr clearProperty0;   // +0x28C StageClear::Property: five shared_ptrs
    guest::SharedPtr clearProperty1;   // +0x294
    guest::SharedPtr clearProperty2;   // +0x29C
    guest::SharedPtr clearProperty3;   // +0x2A4
    guest::SharedPtr clearProperty4;   // +0x2AC
    guest::SharedPtr clearData;   // +0x2B4 the clear response (object, control block)
    rw::Field<uint8_t> showGettingItem;   // +0x2BC
    rw::Field<uint8_t> showClearRank;   // +0x2BD
    rw::Field<uint8_t> showLevelUp;   // +0x2BE
    rw::Field<uint8_t> showNewLux;   // +0x2BF
    rw::Field<uint8_t> showUserTitle;   // +0x2C0
    rw::Field<uint8_t> showNewKeyblade;   // +0x2C1
    const uint8_t pad_2C2[0x2];
    rw::Field<uint32_t> keybladeIndex;   // +0x2C4
    rw::PtrField<uint32_t> dialog;   // +0x2C8 BattleResultDialog*
    guest::Vector rewardKeyblades;   // +0x2CC std::vector<BattleResultScrollView::ResutData> (0x44 bytes each)
};
RW_LAYOUT(SceneBattleResultLayout, vtable, 0x0);
RW_LAYOUT(SceneBattleResultLayout, onEnterCallback, 0x210);
RW_LAYOUT(SceneBattleResultLayout, onExitCallback, 0x228);
RW_LAYOUT(SceneBattleResultLayout, resultKind, 0x288);
RW_LAYOUT(SceneBattleResultLayout, clearProperty0, 0x28C);
RW_LAYOUT(SceneBattleResultLayout, clearProperty1, 0x294);
RW_LAYOUT(SceneBattleResultLayout, clearProperty2, 0x29C);
RW_LAYOUT(SceneBattleResultLayout, clearProperty3, 0x2A4);
RW_LAYOUT(SceneBattleResultLayout, clearProperty4, 0x2AC);
RW_LAYOUT(SceneBattleResultLayout, clearData, 0x2B4);
RW_LAYOUT(SceneBattleResultLayout, showGettingItem, 0x2BC);
RW_LAYOUT(SceneBattleResultLayout, showClearRank, 0x2BD);
RW_LAYOUT(SceneBattleResultLayout, showLevelUp, 0x2BE);
RW_LAYOUT(SceneBattleResultLayout, showNewLux, 0x2BF);
RW_LAYOUT(SceneBattleResultLayout, showUserTitle, 0x2C0);
RW_LAYOUT(SceneBattleResultLayout, showNewKeyblade, 0x2C1);
RW_LAYOUT(SceneBattleResultLayout, keybladeIndex, 0x2C4);
RW_LAYOUT(SceneBattleResultLayout, dialog, 0x2C8);
RW_LAYOUT(SceneBattleResultLayout, rewardKeyblades, 0x2CC);
RW_SIZE(SceneBattleResultLayout, 0x2D8);

struct SceneBattleResult : SceneBattleResultLayout {
    static SceneBattleResult *create();
    static SceneBattleResult *destruct(SceneBattleResult *self);       // D2
    static uint32_t init(SceneBattleResult *self);
    static void initScene(SceneBattleResult *self);
    static void nextResult(SceneBattleResult *self);
    static void openLevelUpPopup(SceneBattleResult *self);
    static void openNewLuxPopup(SceneBattleResult *self, int32_t rank);
    static void startResult(SceneBattleResult *self);
    static void addResultRewardKeyBlade(SceneBattleResult *self, int32_t rewardId, int32_t index);
};
