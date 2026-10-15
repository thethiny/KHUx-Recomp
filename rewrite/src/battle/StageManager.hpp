// StageManager: the running quest's stage and map data (a cocos2d::Ref, created by StageManager::create at the
// start of a quest). It reads the stage file (stage/mappoi_stg%05d.bin, or stage/cl_stg%08d.bin in the
// Coliseum), then for the current map the map file (stage/mappoi_stg%05d_%02d.bin) and the map's resource files
// (map/<name>/<name>_info/_jmp/_cls/_chp.bin), and turns them into the map's enemy teams, transfer gates,
// treasures, materials, NPCs, jump points and field layers (StageMapInfo). It also evaluates the stage's sub
// missions at the end of the quest.
//
// The stage / map files are little-endian records read byte by byte; offsets inside them are noted per field.
#pragma once

#include "BattleCommon.hpp"

struct StageManager {
    // cocos2d::Data: a byte buffer (bytes, size).
    struct Data {
        rw::Field<uint32_t> bytes;  // +0
        rw::Field<uint32_t> size;   // +4
    };

    // std::unordered_map (libc++, 32-bit): buckets, bucket count, first node (__p1_.__next_), size, max load factor.
    // A node: next, hash, key, value (value at +0xC for an int key and a pointer value).
    struct HashMap {
        rw::Field<uint32_t> buckets;        // +0x00
        rw::Field<uint32_t> bucketCount;    // +0x04
        rw::Field<uint32_t> first;          // +0x08
        rw::Field<uint32_t> size;           // +0x0C
        rw::Field<float> maxLoadFactor;     // +0x10
    };

    // StageManager::StageInfo::Param: a SafeBuffer of 4 bytes (own vtable). Story: 0 (enemy level offset,
    // added to every enemy symbol's level); Coliseum: the rise level (getColosseumRiseLevel).
    struct StageInfoParam : SafeBuffer {
        static constexpr uint32_t VTABLE = 0xF97740;   // _ZTVN12StageManager9StageInfo5ParamE + 8
        static void destructDelete(StageInfoParam *self);
    };

    // StageManager::StageMapInfo: the current map (232 bytes).
    struct StageMapInfo {
        rw::Field<int32_t> mapId;             // +0x00 map number inside the stage
        const uint8_t mapName[16];                  // +0x04 resource name (map/<name>/...), copied from the map file
        Data mapData;                         // +0x14 the map file (stage/mappoi_stg%05d_%02d.bin)
        rw::Field<uint32_t> enemyOffset;      // +0x1C section offsets in mapData (file value + 52, the header)
        rw::Field<uint32_t> transferOffset;   // +0x20
        rw::Field<uint32_t> treasureOffset;   // +0x24
        rw::Field<uint32_t> materialOffset;   // +0x28
        rw::Field<uint32_t> npcOffset;        // +0x2C
        bt::Vector enemyTeams;                // +0x30 vector<MapEnemyTeamInfo*> (teams without a random symbol)
        bt::Vector transfers;                 // +0x3C vector<MapTransferInfo*>
        bt::Vector treasures;                 // +0x48 vector<MapTreasure::Info*>
        bt::Vector materials;                 // +0x54 vector<MapMaterial::Info*>
        bt::Vector npcs;                      // +0x60 vector<MapNonPlayableCharacter::Info*>
        rw::Field<uint32_t> teamArray;        // +0x6C new[] MapEnemyTeamInfo (68 bytes each)
        rw::Field<uint32_t> symbolArray;      // +0x70 new[] enemy symbols (32 bytes each)
        rw::Field<uint32_t> transferArray;    // +0x74 new[] MapTransferInfo (36 bytes each)
        rw::Field<uint32_t> treasureArray;    // +0x78 new[] MapTreasure::Info (24 bytes each)
        rw::Field<uint32_t> materialArray;    // +0x7C new[] MapMaterial::Info (20 bytes each)
        rw::Field<uint32_t> npcArray;         // +0x80 new[] MapNonPlayableCharacter::Info (36 bytes each)
        Data clsData;                         // +0x84 map/<name>/<name>_cls.bin (collision)
        Data chpData;                         // +0x8C map/<name>/<name>_chp.bin
        HashMap jumps;                        // +0x94 unordered_map<int, vector<MapJumpInfo*>*>
        rw::Field<int32_t> field_A8;          // +0xA8 16-bit value at map file +0x1C
        rw::Field<uint8_t> noBaseFlag;        // +0xAC 1 when the _info.bin word at +8 is 0
        const uint8_t pad_AD[3];
        HashMap fieldLayers;                  // +0xB0 unordered_map<int, MapFieldLayer*> (id 0 = "base1")
        bt::Vector placements;                // +0xC4 vector<MapPlacementObject*>: every placement (retained)
        bt::Vector unlayeredPlacements;       // +0xD0 vector<MapPlacementObject*>: placements of layer 0 / slot 0
        bt::Vector modeNodes;                 // +0xDC vector<MapNode*>: nodes that fade with changeMode

        static StageMapInfo *construct(StageMapInfo *self);
    };

    rw::Field<uint32_t> vtable;               // +0x00 cocos2d::Ref
    const uint8_t pad_04[0x10];                     // +0x04 Ref: reference count, script binding ids
    rw::Field<int32_t> mode;                  // +0x14 0 = story quest, 1 = Coliseum
    rw::Field<int32_t> state;                 // +0x18 2 after init
    const uint8_t pad_1C[4];
    rw::Field<int32_t> stageId;               // +0x20
    rw::Field<int32_t> mapTotalNumber;        // +0x24 stage file +0x08 (16 bits)
    rw::Field<int32_t> bossEnemyId;           // +0x28 stage file +0x18
    rw::Field<int32_t> enemyPlatoonCount;     // +0x2C stage file +0x20 (story: minus the random symbol count)
    rw::Field<int32_t> field_30;              // +0x30 stage file +0x24 (sub mission 2: minus field_34)
    rw::Field<int32_t> field_34;              // +0x34 stage file +0x34
    rw::Field<int32_t> treasureCount;         // +0x38 stage file +0x38 (sub mission 17)
    rw::Field<int32_t> materialCount;         // +0x3C stage file +0x3C (sub mission 18)
    StageInfoParam param;                     // +0x40
    StageMapInfo map;                         // +0x60
    rw::Field<int32_t> prevMapId;             // +0x148 -1, then the map left by execTransitionMap
    bt::Vec2 startPos;                        // +0x14C player start (stage file +0x0C/+0x10, or a transfer gate)
    rw::Field<int32_t> nextMapId;             // +0x154 map to load (stage file +0x0A, or a transfer gate)
    rw::Field<int32_t> startDirection;        // +0x158 1 / -1 / 0 (file direction 1..4 through a table)
    rw::Field<uint32_t> stageDataFile;        // +0x15C format of the stage file name (char*)
    rw::Field<uint32_t> mapDataFile;          // +0x160 format of the map file name (char*)
    rw::Field<float> misc107;                 // +0x164 master::Misc 107
    rw::Field<float> misc108;                 // +0x168 master::Misc 108
    rw::Field<float> misc109;                 // +0x16C master::Misc 109

    static constexpr uint32_t VTABLE = 0xFA84A4;    // _ZTV12StageManager + 8 (slot 1 delete, slot 2 init)

    // lifetime
    static uint32_t create();
    static StageManager *construct(StageManager *self);
    static StageManager *destruct(StageManager *self);
    static void destructDelete(StageManager *self);
    static uint32_t init(StageManager *self);

    // stage / map loading
    static void getMasterMiscValue(StageManager *self);
    static void stageFirstSetup(StageManager *self, int32_t stageId);
    static void setNextMapInfo(StageManager *self);
    static void clearMapInfo(StageManager *self);
    static void execTransitionMap(StageManager *self);
    static int32_t getColosseumRiseLevel(StageManager *self);
    static void createMapEnemyInfo(StageManager *self, StageMapInfo *info, int32_t levelOffset);
    static void createMapTransferInfo(StageManager *self, StageMapInfo *info);
    static void createMapTreasureInfo(StageManager *self, StageMapInfo *info);
    static void createMapMaterialInfo(StageManager *self, StageMapInfo *info);
    static void createMapNpcInfo(StageManager *self, StageMapInfo *info);
    static void createMapJumpInfo(StageManager *self, StageMapInfo *info);
    static void createMapFieldLayer(StageManager *self, StageMapInfo *info);
    static void deleteMapEnemyInfo(StageManager *self, StageMapInfo *info);
    static void deleteMapTransferInfo(StageManager *self, StageMapInfo *info);
    static void deleteMapTreasureInfo(StageManager *self, StageMapInfo *info);
    static void deleteMapMaterialInfo(StageManager *self, StageMapInfo *info);
    static void deleteMapNpcInfo(StageManager *self, StageMapInfo *info);
    static void deleteMapJumpInfo(StageManager *self, StageMapInfo *info);
    static void deleteMapFieldLayer(StageManager *self, StageMapInfo *info);
    static int32_t setupEncountArea(StageManager *self, bt::Vec2 *area, const uint8_t *bytes);
    static uint32_t hasRandomEnemySymbol(StageManager *self, uint32_t symbolId);

    // file helpers
    static int32_t getInteger16bitFromLittleEndianBytes(const uint8_t *bytes);
    static int32_t getInteger32bitFromLittleEndianBytes(const uint8_t *bytes);
    static int32_t convertDirectionID(StageManager *self, int32_t direction);
    static uint32_t convertIsFixing(StageManager *self, int32_t direction);
    static int32_t createUniqueIdInStage(StageManager *self, int32_t mapId, int32_t index, int32_t kind);

    // queries
    static int32_t getWorldCode(StageManager *self);
    static uint32_t getMapID(StageManager *self);
    static int32_t getMapTotalNumber(StageManager *self);
    static uint32_t getMapStartInfo(StageManager *self);
    static int32_t getStageBossEnemyID(StageManager *self);
    static int32_t getStageEnemyPlatoonCount(StageManager *self);
    static uint32_t getAvatarActorStylePlayable(StageManager *self, uint32_t style);
    static uint32_t getAvatarActorStyleFriend(StageManager *self, uint32_t style);
    static uint32_t getEnemyPlatoonInfo(StageManager *self);
    static uint32_t getEnemyTeamInfo(StageManager *self, int32_t index);
    static int32_t getEnemyPlatoonCount(StageManager *self);
    static uint32_t isInsideTeamEncountArea(StageManager *self, uint32_t pos, int32_t index);
    static uint32_t checkMapTransportGate(StageManager *self, uint32_t pos);
    static uint32_t MarkingMapTransportGate(StageManager *self, uint32_t pos);
    static void changeMode(StageManager *self, uint32_t mode);

    // sub missions
    static void setupSubMission(StageManager *self);
    static void finish(StageManager *self, uint32_t deck);
    static void decisionSubMission(StageManager *self, uint32_t deck);
    static uint32_t isNoBadStatus(StageManager *self);
    static int32_t countOnDeckMedalAttribute(StageManager *self, uint32_t deck, uint32_t attr1, uint32_t attr2,
                                             uint32_t attr3);
    static int32_t getOnDeckMedalCount(StageManager *self, uint32_t deck);
    static uint32_t isEqualKeyBleadKind(StageManager *self, uint32_t keybladeId);
    static int32_t getMostHigherRarity(StageManager *self, uint32_t deck);
    static uint32_t isFullSetTargetRarity(StageManager *self, uint32_t deck, int32_t rarity);
};
RW_SIZE(StageManager::Data, 8);
RW_SIZE(StageManager::HashMap, 20);
RW_SIZE(StageManager::StageInfoParam, 32);
RW_LAYOUT(StageManager::StageMapInfo, mapData, 0x14);
RW_LAYOUT(StageManager::StageMapInfo, enemyTeams, 0x30);
RW_LAYOUT(StageManager::StageMapInfo, teamArray, 0x6C);
RW_LAYOUT(StageManager::StageMapInfo, clsData, 0x84);
RW_LAYOUT(StageManager::StageMapInfo, jumps, 0x94);
RW_LAYOUT(StageManager::StageMapInfo, noBaseFlag, 0xAC);
RW_LAYOUT(StageManager::StageMapInfo, fieldLayers, 0xB0);
RW_LAYOUT(StageManager::StageMapInfo, modeNodes, 0xDC);
RW_SIZE(StageManager::StageMapInfo, 0xE8);
RW_LAYOUT(StageManager, mode, 0x14);
RW_LAYOUT(StageManager, stageId, 0x20);
RW_LAYOUT(StageManager, param, 0x40);
RW_LAYOUT(StageManager, map, 0x60);
RW_LAYOUT(StageManager, prevMapId, 0x148);
RW_LAYOUT(StageManager, startPos, 0x14C);
RW_LAYOUT(StageManager, stageDataFile, 0x15C);
RW_LAYOUT(StageManager, misc109, 0x16C);
RW_SIZE(StageManager, 0x170);
