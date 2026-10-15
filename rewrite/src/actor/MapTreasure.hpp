// MapTreasure: a treasure chest on the quest map (ActMapTreasureBoxAnim). Normal / rare / special chests, each
// plain, HP or ability ("Treasure_R", "TreasureHP_S_Open", ...). The prizes are rolled when the chest is built
// (StageUtil::getTreasurePrizes) into a hidden MapPrizes burst; open() plays the opening animation and, when its
// "Open" frame event fires, terminate() erupts the prizes and calls the open callback.
#pragma once

#include "FunctionThunks.hpp"
#include "FlashNode.hpp"   // StdFunction
#include "core/SafeBuffer.hpp"

struct MapTreasure : MapNode {
    // The chest to build (argument of create / init).
    struct Info {
        bt::Vec2 position;                 // +0
        rw::Field<int32_t> boxType;        // +8  animation: rarity (boxType % 6) / 2 (N, R, S), kind boxType / 6
                                           //     (plain, HP, ability); even = mirrored
        rw::Field<int32_t> treasureId;     // +12
        rw::Field<int32_t> rewardId;       // +16
        rw::Field<uint8_t> alreadyOpened;  // +20 shown opened ("*_Open"), no prizes
    };

    // MapTreasure::Property: SafeBuffer of 8 bytes: treasure id (0), reward id (4).
    struct Property : SafeBuffer {
        static constexpr uint32_t VTABLE = 0xF93494;   // _ZTVN11MapTreasure8PropertyE + 8
        static constexpr uint32_t TREASURE_ID = 0;
        static constexpr uint32_t REWARD_ID = 4;
        static void destructDelete(Property *self);
    };

    enum State : int32_t { CLOSED = 0, OPENING = 1, OPENED = 2, ALREADY_OPENED = 3 };

    Property property;                     // +648
    rw::Field<uint32_t> armature;          // +680 cocostudio::Armature* ActMapTreasureBoxAnim
    bt::Vector prizes;                     // +684 cocos2d::Vector<MapPrizes*>
    rw::Field<int32_t> state;              // +696 State (StageFieldManager CHEST_OPENED 0x2B8)
    rw::Field<uint32_t> field_2BC;         // +700 cleared by the constructor
    StdFunction onOpened;                  // +704 std::function<void(MapTreasure*)> called by terminate()
    const uint8_t pad_2d4[4];                    // +724

    static constexpr uint32_t VTABLE = 0xF931E4;   // _ZTV11MapTreasure + 8
    static constexpr uint32_t SIZE = 0x2D8;
    static constexpr uint32_t VSLOT_INIT = 156;    // virtual init(const Info*, cocos2d::Node*, FromStageType)
    static constexpr int32_t NODE_TAG = 6;         // node kind on the field: treasure

    static MapTreasure *construct(MapTreasure *self);
    static MapTreasure *destruct(MapTreasure *self);
    static void destructDelete(MapTreasure *self);
    static uint32_t init(MapTreasure *self, const Info *info, uint32_t parent, int32_t fromStageType);
    static MapTreasure *create(const Info *info, uint32_t parent, int32_t fromStageType);
    static void firstCall();
    static void finalCall();
    static void terminate(MapTreasure *self);
    static uint32_t open(MapTreasure *self, uint32_t onOpened);

    // init()'s frame-event lambda ($_1, captures this): "Open" -> terminate()
    static void onFrameEvent(uint32_t lambda, uint32_t bone, uint32_t eventName);
};
RW_LAYOUT(MapTreasure::Info, boxType, 8);
RW_LAYOUT(MapTreasure::Info, alreadyOpened, 20);
RW_LAYOUT(MapTreasure, property, 648);
RW_LAYOUT(MapTreasure, armature, 680);
RW_LAYOUT(MapTreasure, prizes, 684);
RW_LAYOUT(MapTreasure, state, 696);
RW_LAYOUT(MapTreasure, onOpened, 704);
RW_SIZE(MapTreasure, 0x2D8);
