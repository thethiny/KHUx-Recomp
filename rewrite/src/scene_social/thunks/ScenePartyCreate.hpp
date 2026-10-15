// ScenePartyCreate: the party creation scene (a cocos2d::Layer with an EditBoxDelegate base at +0x288 and an
// IInOutAnimation base at +0x28C). Over the PartyTopScene layout it opens the PartyTop_Edit popup: party name and
// message edit boxes, play style and join type check boxes, OK / close buttons; OK creates (or updates) the party.
#pragma once

#include "../../core/Guest.hpp"
#include "../../scene/SceneUi.hpp"

struct ScenePartyCreate {
    using Addr = uint32_t;

    const uint8_t layer[0x210];                // cocos2d::Layer
    const uint8_t onEnterCallback[24];         // +0x210 std::function<void()> (scheduleUpdate)
    const uint8_t onExitCallback[24];          // +0x228 std::function<void()> (unscheduleUpdate)
    const uint8_t pad_240[0x288 - 0x240];
    rw::Field<uint32_t> vtableEditBoxDelegate; // +0x288 cocos2d::extension::EditBoxDelegate
    rw::Field<uint32_t> vtableInOut;           // +0x28C IInOutAnimation
    rw::Field<uint32_t> sceneNode;             // +0x290 the PartyTopScene layout
    rw::Field<int32_t> playStyle;              // +0x294 1..3 (the play style check boxes)
    rw::Field<int32_t> joinType;               // +0x298 1..3 (the join type check boxes)
    rw::Field<uint32_t> editNode;              // +0x29C the edit box being edited (SceneUtil text field helpers)
    rw::Field<uint32_t> editLabel;             // +0x2A0 its label
    rw::Field<uint8_t> editing;                // +0x2A4 a request / edit is in progress
    const uint8_t pad_2A5[3];
    rw::Field<uint32_t> messageText;           // +0x2A8 ui::Text showing the message
    const uint8_t pad_2AC[4];

    static constexpr uint32_t SIZE = 0x2B0;
    static constexpr uint32_t VTABLE_BASE = 0xFA05E0;     // _ZTV16ScenePartyCreate
    static constexpr uint32_t VTABLE = VTABLE_BASE + 8;
    static constexpr uint32_t VTABLE2 = VTABLE_BASE + 0x328;   // +0x288
    static constexpr uint32_t VTABLE3 = VTABLE_BASE + 0x348;   // +0x28C

    static Addr create();
    static Addr createScene();
    static Addr destroy(ScenePartyCreate *self);
    static Addr destroyThunk648(Addr base);
    static Addr destroyThunk652(Addr base);
    static void destroyDelete(ScenePartyCreate *self);
    static void destroyDeleteThunk648(Addr base);
    static void destroyDeleteThunk652(Addr base);
    static uint32_t init(ScenePartyCreate *self);
    static void openPopUpCreateParty(ScenePartyCreate *self);
    static void update(ScenePartyCreate *self, float dt);
    static void runInAnimation(ScenePartyCreate *self);
    static void runInAnimationThunk652(Addr base);
    static void runOutAnimationMain(ScenePartyCreate *self, Addr done);
    static void runOutAnimationMainThunk652(Addr base, Addr done);
    static void editBoxEditingDidEnd(ScenePartyCreate *self, Addr editBox);
    static void editBoxEditingDidEndThunk648(Addr base, Addr editBox);
    static void editBoxEditingDidBegin(ScenePartyCreate *self, Addr editBox);
    static void editBoxTextChanged(ScenePartyCreate *self, Addr editBox, Addr text);
    static void editBoxReturn(ScenePartyCreate *self, Addr editBox);
};
RW_LAYOUT(ScenePartyCreate, vtableEditBoxDelegate, 0x288);
RW_LAYOUT(ScenePartyCreate, sceneNode, 0x290);
RW_LAYOUT(ScenePartyCreate, editing, 0x2A4);
RW_LAYOUT(ScenePartyCreate, messageText, 0x2A8);
RW_SIZE(ScenePartyCreate, ScenePartyCreate::SIZE);
