// StageFieldManager: the quest map ("field") of a stage. Owns the cocos2d root node every map layer is added to,
// the map node lists by kind (prizes, materials, treasures, NPCs, gate markers), the camera (scroll position,
// bounds, zoom), the collision / jump grids (cocos2d::Data bitmaps owned by StageManager), the event under the
// player (treasure / NPC tapped) and the three "attract" targets prizes fly to when collected.
#pragma once

#include "BattleCommon.hpp"

// cocos2d::Rect: origin, size.
struct SfmRect {
    bt::Vec2 origin;
    bt::Vec2 size;
};
RW_SIZE(SfmRect, 16);

// StageFieldManager::AttractTarget: a node (created at the HUD target, in field coordinates) plus its offset from
// the camera position.
struct AttractTarget {
    rw::Field<uint32_t> node;   // +0 cocos2d::Node*
    bt::Vec2 offset;            // +4 HUD position - screen centre, scaled by 1/1.2, y flipped
};
RW_SIZE(AttractTarget, 12);

struct StageFieldManager {
    rw::Field<uint32_t> vtable;            // +0x00
    rw::Field<uint32_t> referenceCount;    // +0x04 cocos2d::Ref
    const uint8_t pad_08[12];                    // +0x08 Ref script-binding fields (_ID, _luaID, _scriptObject)
    rw::Field<uint32_t> fieldNode;         // +0x14 cocos2d::Node* root of the field (retained)
    rw::Field<uint32_t> mainLayer;         // +0x18 MapFieldLayer* (layer id 0): size at +0x6B0 / +0x6B4
    bt::Vector layers;                     // +0x1C cocos2d::Vector<MapFieldLayer*>
    rw::Field<uint32_t> jumpInfoMap;       // +0x28 unordered_map<int, vector<MapJumpInfo*>*>* (StageManager+0xF4)
    rw::Field<uint32_t> collisionData;     // +0x2C cocos2d::Data* 1 bit per pixel, 1 = blocked (StageManager+0xE4)
    rw::Field<uint32_t> jumpData;          // +0x30 cocos2d::Data* one byte per 60x60 cell, 2 = jump (StageManager+0xEC)
    rw::Field<int32_t> collisionStride;    // +0x34 bytes per collision row: (width + 7) / 8
    rw::Field<int32_t> jumpStride;         // +0x38 cells per jump row: (width + 59) / 60
    bt::Vector nodes;                      // +0x3C cocos2d::Vector<MapNode*> every node on the field
    bt::Vector prizes;                     // +0x48 Vector<MapPrizes*>      (node kind 4)
    bt::Vector materials;                  // +0x54 Vector<MapMaterial*>    (kind 5)
    bt::Vector treasures;                  // +0x60 Vector<MapTreasure*>    (kind 6)
    bt::Vector npcs;                       // +0x6C Vector<MapNonPlayableCharacter*> (kind 10)
    bt::Vector gateMarkers;                // +0x78 Vector<MapGateMarker*>  (kind 8)
    rw::Field<int32_t> zoomState;          // +0x84 current zoom (0 normal, 1 zoomed in)
    rw::Field<int32_t> zoomRequest;        // +0x88 requested zoom
    bt::Vec2 screenCenter;                 // +0x8C half the visible size
    bt::Vec2 cameraPos;                    // +0x94 map position at the screen centre
    SfmRect cameraBounds;                  // +0x9C range the camera position may take
    SfmRect mapRect;                       // +0xAC (0, 0, map size)
    rw::Field<float> materialRange;        // +0xBC pick-up distance of materials (StageManager+0x16C)
    rw::Field<int32_t> eventType;          // +0xC0 0 none, 1 treasure, 2 NPC (set by a tap)
    bt::Vec2 eventPos;                     // +0xC4 where the event object stands
    rw::Field<float> eventRange;           // +0xCC distance the player must reach (100 treasure, 120 NPC)
    rw::Field<uint32_t> eventTarget;       // +0xD0 MapTreasure* / MapNonPlayableCharacter*
    rw::Field<uint32_t> talkingNpc;        // +0xD4 NPC of the running talk
    AttractTarget luxTarget;               // +0xD8 Lux prizes (item type 11)
    AttractTarget flaggedItemTarget;       // +0xE4 items whose Item flag is set (setTreasureTarget 2nd node)
    AttractTarget itemTarget;              // +0xF0 other items (setTreasureTarget 1st node)
    rw::Field<uint32_t> weather;           // +0xFC SnowParticle* (snow weather) or 0

    static constexpr uint32_t VTABLE = 0xFA8454;   // _ZTV17StageFieldManager + 8
    static constexpr uint32_t SIZE = 0x100;

    static StageFieldManager *create();
    static StageFieldManager *construct(StageFieldManager *self);
    static StageFieldManager *destruct(StageFieldManager *self);
    static void destructDelete(StageFieldManager *self);
    static void removeField(StageFieldManager *self);
    static uint32_t init(StageFieldManager *self);
    static void setStageRaid(StageFieldManager *self, uint32_t layer);
    static uint32_t setStageMapInfo(StageFieldManager *self, uint32_t layer, uint32_t stageManager);
    static void addMapNodeToField(StageFieldManager *self, uint32_t node);
    static uint32_t getWeatherNode(StageFieldManager *self, uint32_t type, bt::Vec2 *size);
    static float getAutoDirectionFromField(StageFieldManager *self, float x);
    static void getCameraMapPosition(bt::Vec2 *out, StageFieldManager *self, bt::Vec2 *pos, float direction);
    static void updateCamera(StageFieldManager *self, bt::Vec2 *pos, float dt);
    static void showMapNode(StageFieldManager *self);
    static void removeMapNodeToField(StageFieldManager *self, uint32_t node);
    static void addNpcToFieldLayer(StageFieldManager *self, uint32_t npc, int32_t layerId, int32_t z);
    static void zoomUpdate(StageFieldManager *self);
    static void update_common(StageFieldManager *self, bt::Vec2 *size);
    static void update_raid(StageFieldManager *self);
    static void update(StageFieldManager *self, float dt, uint32_t actor);
    static void checkMaterial(StageFieldManager *self, bt::Vec2 *pos);
    static void addPrize(StageFieldManager *self, uint32_t prize);
    static void delPrize(uint32_t self, uint32_t prize);
    static void getLocalPosToMapPos(bt::Vec2 *out, StageFieldManager *self, bt::Vec2 *pos);
    static void getScreenPosToMapPos(bt::Vec2 *out, StageFieldManager *self, bt::Vec2 *pos);
    static void getMapPosToScreenPos(bt::Vec2 *out, StageFieldManager *self, bt::Vec2 *pos);
    static uint32_t fitRectToPoint(uint32_t self, bt::Vec2 *p, SfmRect *rect);
    static uint32_t isCollisionRectSafe(StageFieldManager *self, bt::Vec2 *pos);
    static uint32_t isCollisionSafe(StageFieldManager *self, int32_t x, int32_t y);
    static uint32_t isCollisionSafeVec(StageFieldManager *self, bt::Vec2 *pos);
    static uint32_t isCollisionSafeF(StageFieldManager *self, float x, float y);
    static uint32_t isCollisionDetectionVec(StageFieldManager *self, bt::Vec2 *from, bt::Vec2 *to);
    static uint32_t isCollisionDetection(StageFieldManager *self, int32_t x0, int32_t y0, int32_t x1, int32_t y1);
    static uint32_t hasJumpData(StageFieldManager *self, uint32_t actor, bt::Vec2 *target);
    static void onOpenTreasure(StageFieldManager *self, uint32_t treasure);
    static uint32_t checkEventAreaTap(StageFieldManager *self, bt::Vec2 *pos);
    static uint32_t checkEventOccurs(StageFieldManager *self, uint32_t stageManager, uint32_t actor);
    static void onEventOccursTreasure(StageFieldManager *self, uint32_t treasure, uint32_t actor);
    static void onEventOccursNpc(StageFieldManager *self, uint32_t npc, uint32_t actor, uint32_t stageManager);
    static uint32_t getCurrentEventMapTreasure(StageFieldManager *self);
    static uint32_t isProcessedPrizes(StageFieldManager *self);
    static void setLuxTarget(StageFieldManager *self, uint32_t target, uint32_t parent);
    static uint32_t setTargetNode(StageFieldManager *self, AttractTarget *target, uint32_t hudNode);
    static void setTreasureTarget(StageFieldManager *self, uint32_t itemNode, uint32_t flaggedNode, uint32_t parent);
    static void clearTargetNode(uint32_t self, AttractTarget *target);
    static void addPrizeAttract(StageFieldManager *self, uint32_t prize, AttractTarget *target, uint32_t flag);
    static void terminateCollectPrize(StageFieldManager *self, uint32_t actor);
    static void changeMode(StageFieldManager *self, uint32_t battle);
    static void reapStateBoard(StageFieldManager *self, float dt, bt::Vector *enemies);
    static uint32_t isHitTestTouchRectAndTouchUI(StageFieldManager *self, uint32_t enemy);
};
RW_LAYOUT(StageFieldManager, fieldNode, 0x14);
RW_LAYOUT(StageFieldManager, layers, 0x1C);
RW_LAYOUT(StageFieldManager, jumpInfoMap, 0x28);
RW_LAYOUT(StageFieldManager, nodes, 0x3C);
RW_LAYOUT(StageFieldManager, gateMarkers, 0x78);
RW_LAYOUT(StageFieldManager, zoomState, 0x84);
RW_LAYOUT(StageFieldManager, screenCenter, 0x8C);
RW_LAYOUT(StageFieldManager, cameraPos, 0x94);
RW_LAYOUT(StageFieldManager, cameraBounds, 0x9C);
RW_LAYOUT(StageFieldManager, mapRect, 0xAC);
RW_LAYOUT(StageFieldManager, materialRange, 0xBC);
RW_LAYOUT(StageFieldManager, eventType, 0xC0);
RW_LAYOUT(StageFieldManager, eventPos, 0xC4);
RW_LAYOUT(StageFieldManager, eventTarget, 0xD0);
RW_LAYOUT(StageFieldManager, luxTarget, 0xD8);
RW_LAYOUT(StageFieldManager, flaggedItemTarget, 0xE4);
RW_LAYOUT(StageFieldManager, itemTarget, 0xF0);
RW_LAYOUT(StageFieldManager, weather, 0xFC);
RW_SIZE(StageFieldManager, 0x100);
