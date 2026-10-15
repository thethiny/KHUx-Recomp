// AvatarIcon: an avatar's face rendered into textures (one RenderTexture per facial expression), shown as a node
// that switches (or cross-fades) between the expressions. Used by the party / ranking lists (AvatarIconCache).
#pragma once

#include "actor/MapNode.hpp"

struct AvatarIcon {
    rw::Field<uint32_t> vtable;             // +0   cocos2d::Node
    const uint8_t node_[620];
    bt::Vector nodes;                       // +624 cocos2d::Vector<Node*>: one RenderTexture per expression
    // std::unordered_map<int, int> expression -> index in nodes (libc++ __hash_table)
    rw::Field<uint32_t> mapBuckets;         // +636
    rw::Field<uint32_t> mapBucketCount;     // +640
    rw::Field<uint32_t> mapFirst;           // +644 first node (__p1_)
    rw::Field<uint32_t> mapSize;            // +648
    rw::Field<float> mapMaxLoadFactor;      // +652 1.0
    rw::Field<int32_t> current;             // +656 index of the expression shown
    const uint8_t pad_294[4];

    static constexpr uint32_t VTABLE = 0xF8F62C;   // _ZTV10AvatarIcon + 8
    static constexpr uint32_t SIZE = 0x298;
};
RW_LAYOUT(AvatarIcon, nodes, 624);
RW_LAYOUT(AvatarIcon, mapBuckets, 636);
RW_LAYOUT(AvatarIcon, mapMaxLoadFactor, 652);
RW_LAYOUT(AvatarIcon, current, 656);
RW_SIZE(AvatarIcon, AvatarIcon::SIZE);
