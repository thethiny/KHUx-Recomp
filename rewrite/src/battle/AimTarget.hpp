// AimTarget: the touch-drag aiming on the quest map (drag from the avatar to choose where to attack / move);
// AimMarker is the animated marker (ActionMapAnimation "MoveMarker-anm") shown while aiming.
#pragma once

#include "BattleCommon.hpp"

struct AimTarget {
    // The marker node (a MapNode).
    struct AimMarker {
        rw::Field<uint32_t> vtable;        // +0  MapNode (cocos2d::Node)
        const uint8_t pad_004[524];
        const uint8_t onShow[20];                // +528 std::function<void()> (MapNode callback, set to a lambda)
        const uint8_t pad_21c[4];
        const uint8_t onHide[20];                // +552 std::function<void()>
        const uint8_t pad_23c[76];
        rw::Field<float> deltaTime;        // +648 time accumulated by update(), taken by takeDeltaTime()
        const uint8_t pad_28c[4];

        static constexpr uint32_t VTABLE = 0xFA797C;   // _ZTVN9AimTarget9AimMarkerE + 8
        static constexpr uint32_t SIZE = 0x290;

        static AimMarker *create();
        static float takeDeltaTime(AimMarker *self);
        static void hide(AimMarker *self);
        static AimMarker *destruct(AimMarker *self);
        static void destructDelete(AimMarker *self);
        static uint32_t init(AimMarker *self);
        static void show(AimMarker *self);
        static void update(AimMarker *self, float dt);
    };

    rw::Field<uint32_t> vtable;            // +0  cocos2d::Ref
    const uint8_t pad_04[16];                    // +4  Ref
    bt::Vec2 touchStart;                   // +20 location in view where the drag started
    bt::Vec2 touchCurrent;                 // +28 current drag location
    rw::Field<uint8_t> field_24;           // +36
    rw::Field<uint8_t> touching;           // +37 a drag is in progress
    rw::Field<uint8_t> active;             // +38 aiming (cleared when the marker times out)
    rw::Field<uint8_t> disabled;           // +39 touches ignored
    rw::Field<uint8_t> stopped;            // +40 stop(): touches ignored until the next touch end
    const uint8_t pad_29[3];
    rw::Field<float> markerTimer;          // +44 seconds the marker stays after release
    rw::Field<float> dragThreshold;        // +48 visible height / 8
    rw::Field<uint32_t> marker;            // +52 AimMarker* (retained)
    const uint8_t callback[20];                  // +56 std::function (callable pointer at +72)
    const uint8_t pad_4c[4];

    static constexpr uint32_t VTABLE = 0xFA7968;   // _ZTV9AimTarget + 8
    static constexpr uint32_t SIZE = 0x50;

    static AimTarget *create();
    static AimTarget *construct(AimTarget *self);
    static AimTarget *destruct(AimTarget *self);
    static void destructDelete(AimTarget *self);
    static uint32_t init(AimTarget *self);
    static void reset(AimTarget *self);
    static void onBegan(AimTarget *self, uint32_t touch);
    static void onMoved(AimTarget *self, uint32_t touch);
    static void onEnded(AimTarget *self, uint32_t touch);
    static void onCancelled(AimTarget *self);
    static void stop(AimTarget *self, uint32_t stopped);
    static void updateMarker(AimTarget *self, float dt, uint32_t keep);
};
RW_LAYOUT(AimTarget, touchStart, 20);
RW_LAYOUT(AimTarget, touching, 37);
RW_LAYOUT(AimTarget, stopped, 40);
RW_LAYOUT(AimTarget, markerTimer, 44);
RW_LAYOUT(AimTarget, marker, 52);
RW_LAYOUT(AimTarget, callback, 56);
RW_SIZE(AimTarget, 0x50);
RW_LAYOUT(AimTarget::AimMarker, onShow, 528);
RW_LAYOUT(AimTarget::AimMarker, onHide, 552);
RW_LAYOUT(AimTarget::AimMarker, deltaTime, 648);
RW_SIZE(AimTarget::AimMarker, 0x290);
