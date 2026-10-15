// MapGateMarker: the "next map" arrow shown over a gate of the quest map once the area is cleared. Two copies of
// map/next_map.png bob up and down / fade out above the gate, always drawn on top (depth locked to FLT_MAX).
#pragma once

#include "MapNode.hpp"

struct MapGateMarker : MapNode {
    rw::Field<uint32_t> arrowRoot;         // +648 cocos2d::Node* holder of both sprites (80 px below the node)
    rw::Field<uint32_t> arrowFront;        // +652 cocos2d::Sprite* (z 1): reappears at y 40, rises and fades out
    rw::Field<uint32_t> arrowGhost;        // +656 cocos2d::Sprite* (z 2): bobs between y 40 and 0
    const uint8_t pad_294[4];                    // +660

    static constexpr uint32_t VTABLE = 0xF92210;   // _ZTV13MapGateMarker + 8
    static constexpr uint32_t SIZE = 0x298;
    static constexpr int32_t NODE_TAG = 8;         // node kind on the field (StageFieldManager: gate markers)

    static MapGateMarker *create();
    static MapGateMarker *destruct(MapGateMarker *self);
    static void destructDelete(MapGateMarker *self);
    static uint32_t init(MapGateMarker *self);
    static void show(MapGateMarker *self);
    static void showAt(MapGateMarker *self, const bt::Vec2 *position, float rotation);
};
RW_LAYOUT(MapGateMarker, arrowRoot, 648);
RW_LAYOUT(MapGateMarker, arrowGhost, 656);
RW_SIZE(MapGateMarker, 0x298);
