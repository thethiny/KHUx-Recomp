// RaidBoss: the raid boss on the raid battle field - a FlashNode playing the boss's LWF animation
// ("lwf/character/raid/..." per motion). Its LWF movies "click_point_N" mark where the player can tap to target a
// part; updateTargetPos / update gather their positions into clickPoints (name -> field position, scaled by
// `scale`), and the "Break" movie of each point is shown while that part is targetable.
#pragma once

#include "../actor/FlashNode.hpp"
#include "BattleUi.hpp"

struct RaidBoss : FlashNode {
    // FlashNode +668 (lwfNode) is the LWFNode* of the current motion; +672 the CommandParser.
    rw::Field<int32_t> bossId;              // +1308 formats the motion's LWF path
    rw::Field<float> scale;                 // +1312 click point positions are multiplied by it (1.0)
    rw::Field<int32_t> motion;              // +1316 RaidBoss::Motion (5 at construction, then 4)
    rw::Field<uint8_t> firstStart;          // +1320 1 until the first motion-4 start trimmed clickPoints
    const uint8_t pad_529[3];
    // std::unordered_map<std::string, cocos2d::Vec2> clickPoints (libc++ __hash_table: buckets, bucket count,
    // first node, size, max_load_factor); node = next, hash, key string (+8), Vec2 value (+20)
    rw::Field<uint32_t> clickBuckets;       // +1324
    rw::Field<uint32_t> clickBucketCount;   // +1328
    rw::Field<uint32_t> clickFirst;         // +1332
    rw::Field<uint32_t> clickSize;          // +1336
    rw::Field<float> clickMaxLoad;          // +1340
    // std::unordered_set<std::string> attackedPoints: parts whose "attack" movie loaded (kept on the first start)
    rw::Field<uint32_t> attackBuckets;      // +1344
    rw::Field<uint32_t> attackBucketCount;  // +1348
    rw::Field<uint32_t> attackFirst;        // +1352
    rw::Field<uint32_t> attackSize;         // +1356
    rw::Field<float> attackMaxLoad;         // +1360
    rw::Field<uint8_t> pointEnabled[6];     // +1364 per click point: its "Break" movie may show
    rw::Field<uint8_t> breakVisible;        // +1370 global switch for the "Break" movies
    const uint8_t pad_55B[5];                     // +1371

    static constexpr uint32_t SIZE = 0x560;
    static constexpr uint32_t VTABLE = 0xF91454;             // _ZTV8RaidBoss + 8
    static constexpr uint32_t CLICK_POINTS_OFFSET = 1324;

    static RaidBoss *create(int32_t bossId);
    static RaidBoss *construct(RaidBoss *self, int32_t bossId);
    static void changeMotion(RaidBoss *self, int32_t motion);
    static void updateTargetPos(RaidBoss *self);
    static void update(RaidBoss *self, float dt);
    static uint32_t getClickPoints(RaidBoss *self);
    static void preloadResources(const RaidBoss *self);
    static RaidBoss *destroy(RaidBoss *self);
    static void deletingDestroy(RaidBoss *self);
};
RW_LAYOUT(RaidBoss, bossId, 1308);
RW_LAYOUT(RaidBoss, clickBuckets, 1324);
RW_LAYOUT(RaidBoss, attackBuckets, 1344);
RW_LAYOUT(RaidBoss, pointEnabled, 1364);
RW_LAYOUT(RaidBoss, breakVisible, 1370);
RW_SIZE(RaidBoss, 0x560);
