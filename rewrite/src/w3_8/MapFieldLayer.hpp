// MapFieldLayer: the quest map's field layer. A MapNode holding the scrolling Field (the tiled map picture, inside
// a container node) and the map nodes that live on it (actors, prizes, gates...). update(scrollX, scrollY) moves
// the camera: the Field picture follows it, and every registered map node is placed at its map position relative
// to the camera centre (map y grows downward, screen y upward).
#pragma once

#include "actor/MapNode.hpp"

struct MapFieldLayer : MapNode {
    // MapFieldLayer::Info (create/init argument)
    struct Info {
        rw::Field<float> mapWidth;        // +0x00 whole map size
        rw::Field<float> mapHeight;       // +0x04
        rw::Field<float> scrollX;         // +0x08 scroll area origin
        rw::Field<float> scrollY;         // +0x0C
        rw::Field<float> fieldWidth;      // +0x10 the picture's size
        rw::Field<float> fieldHeight;     // +0x14
        rw::Field<uint32_t> world;        // +0x18 const char*: the map folder ("map/<world>/<world>_<area>/...")
        rw::Field<uint32_t> area;         // +0x1C const char*
        rw::Field<uint32_t> userValue;    // +0x20 kept as is
        rw::Field<uint8_t> cascadeOpacity;  // +0x24 setCascadeOpacityEnabled argument
    };

    rw::Field<uint32_t> container;        // +648  cocos2d::Node* holding the Field
    rw::Field<uint32_t> field;            // +652  Field*
    // MapInfo handed to Field::create: picture size in pixels and the tile path pattern
    rw::Field<int32_t> infoWidth;         // +656
    rw::Field<int32_t> infoHeight;        // +660
    rw::Field<uint32_t> infoPath;         // +664  const char* (pathBuffer's text)
    const uint8_t pathBuffer[1024];       // +668  StringBuffer: "map/%s/%s_%s/%s_%s_%%03d.png"
    rw::Field<uint32_t> pathLength;       // +1692 StringBuffer length
    bt::Vec2 scrollOrigin;                // +1696 cocos2d::Rect: scroll area (origin, size)
    rw::Field<float> scrollWidth;         // +1704
    rw::Field<float> scrollHeight;        // +1708
    rw::Field<float> mapWidth;            // +1712 cocos2d::Size
    rw::Field<float> mapHeight;           // +1716
    rw::Field<float> fieldWidth;          // +1720 cocos2d::Size
    rw::Field<float> fieldHeight;         // +1724
    rw::Field<uint32_t> userValue;        // +1728
    rw::Field<uint32_t> nodesBegin;       // +1732 cocos2d::Vector<MapNode*> (std::vector of retained nodes)
    rw::Field<uint32_t> nodesEnd;         // +1736
    rw::Field<uint32_t> nodesCap;         // +1740

    static constexpr uint32_t VTABLE = 0xF91F88;   // _ZTV13MapFieldLayer + 8
    static constexpr uint32_t SIZE = 0x6D0;
    static constexpr int32_t NODE_TAG = 13;

    static MapFieldLayer *construct(MapFieldLayer *self);
    static MapFieldLayer *destruct(MapFieldLayer *self);
    static void destructDelete(MapFieldLayer *self);
    static MapFieldLayer *create(const Info *info);
    static uint32_t init(MapFieldLayer *self, const Info *info);
    static void remove(MapFieldLayer *self);
    static uint32_t update(MapFieldLayer *self, float scrollX, float scrollY);
    static void addFieldMapNode(MapFieldLayer *self, MapNode *node, int32_t z);
    static void removeFieldMapNode(MapFieldLayer *self, MapNode *node);
    static void addLayerMapNode(MapFieldLayer *self, MapNode *node, int32_t z);
    static void removeLayerMapNode(MapFieldLayer *self, MapNode *node);
};
RW_LAYOUT(MapFieldLayer, container, 648);
RW_LAYOUT(MapFieldLayer, infoWidth, 656);
RW_LAYOUT(MapFieldLayer, pathBuffer, 668);
RW_LAYOUT(MapFieldLayer, pathLength, 1692);
RW_LAYOUT(MapFieldLayer, scrollOrigin, 1696);
RW_LAYOUT(MapFieldLayer, mapWidth, 1712);
RW_LAYOUT(MapFieldLayer, fieldWidth, 1720);
RW_LAYOUT(MapFieldLayer, userValue, 1728);
RW_LAYOUT(MapFieldLayer, nodesBegin, 1732);
RW_SIZE(MapFieldLayer, 0x6D0);
