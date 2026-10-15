// MapNonPlayableCharacter: an NPC standing on the quest map (a MapNode holding an Npc flash actor). NPCs of talk
// type 1 or 3 show a bobbing talk icon above their head; tapping them starts the talk given by get_talk().
#pragma once

#include "actor/MapNode.hpp"
#include "core/SafeBuffer.hpp"

struct MapNonPlayableCharacter : MapNode {
    // The NPC to place (StageManager's npcArray: 36 bytes each).
    struct Info {
        bt::Vec2 position;                 // +0  map position
        rw::Field<int32_t> npcId;          // +8  Npc::create id
        rw::Field<uint16_t> talkType;      // +12 1: two talks (second one once flagged), 3: one talk
        rw::Field<uint16_t> direction;     // +14 0xFFFF: faces left
        rw::Field<int32_t> talkId;         // +16
        rw::Field<int32_t> talkId2;        // +20 talk 1 once useSecondTalk is set
        rw::Field<int32_t> property;       // +24 stored in the Property buffer
        rw::Field<int32_t> zOffset;        // +28 added to the map y for the depth
        rw::Field<uint16_t> pad_20;        // +32
        rw::Field<uint16_t> flags;         // +34 bit 0: fixed direction (lookAt does nothing), bit 1: second talk
    };

    // MapNonPlayableCharacter::Property: a 4-byte SafeBuffer (Info::property at position 0).
    struct Property : SafeBuffer {
        static constexpr uint32_t VTABLE = 0xF929C0;   // _ZTVN23MapNonPlayableCharacter8PropertyE + 8
    };

    Property property;                     // +648
    rw::Field<uint32_t> talkIconNode;      // +680 cocos2d::Node* (retained) following the NPC, z 100, hidden
    rw::Field<uint32_t> talkIcon;          // +684 cocos2d::Sprite* NPCTalkIcon.png (retained), child of talkIconNode
    rw::Field<uint32_t> npc;               // +688 Npc*
    rw::Field<int32_t> talkType;           // +692
    rw::Field<int32_t> talkId;             // +696
    rw::Field<int32_t> talkId2;            // +700
    rw::Field<int32_t> useSecondTalk;      // +704
    rw::Field<int32_t> npcId;              // +708
    rw::Field<uint8_t> fixedDirection;     // +712
    const uint8_t pad_2C9[7];

    static constexpr uint32_t VTABLE = 0xF92738;   // _ZTV23MapNonPlayableCharacter + 8
    static constexpr uint32_t SIZE = 0x2D0;
    static constexpr uint32_t VSLOT_INIT = 156;    // virtual init(const Info*)
    static constexpr int32_t NODE_TAG = 10;        // node kind on the field: NPC
};
RW_LAYOUT(MapNonPlayableCharacter::Info, flags, 34);
RW_LAYOUT(MapNonPlayableCharacter, property, 648);
RW_LAYOUT(MapNonPlayableCharacter, talkIconNode, 680);
RW_LAYOUT(MapNonPlayableCharacter, npcId, 708);
RW_LAYOUT(MapNonPlayableCharacter, fixedDirection, 712);
RW_SIZE(MapNonPlayableCharacter, 0x2D0);
