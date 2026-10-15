// FlashNode: a cocos2d::Node that plays a character's LWF ("flash") animation - the base of Avatar (the player's
// and friends' avatars), Enemy and Npc. It owns:
//  - the LWF node (+668) and colour helpers (tint / colour transform of the "char" movie),
//  - a CommandParser (+672) whose std::function slots receive the movie's script commands (play a sound effect,
//    change the BGM, queue a knockback / action flag for the stage actor manager),
//  - the bad status display: a status (0 poison, 1 sleep, 2 paralysis, 3 deep poison, 4 none) shown with the
//    ActMapBuffDeBuff_EffectAnim armature (+1296) and a tint of the LWF node, plus the active status bits (+1300:
//    1 poison, 2 sleep, 4 paralysis, 8 deep poison) - several can be active, the display cycles through them,
//  - queues of knockback values (std::deque<int>, +1224) and action flags (std::deque<ActFlag>, +1248).
#pragma once

#include "MapNode.hpp"

// std::function<R(A...)> (libc++, 32-bit): 16-byte in-place buffer, then the callable pointer (points back into
// the buffer for small functors, 0 when empty).
struct StdFunction {
    const uint8_t buf[16];
    rw::Field<uint32_t> f;                 // +16 __base<...>* (0 = empty)
};
RW_SIZE(StdFunction, 20);

// std::deque<T> (libc++, 32-bit): the block map (a __split_buffer: first, begin, end, end of capacity), the index
// of the first element and the size. Blocks hold 1024 4-byte elements.
struct StdDeque {
    rw::Field<uint32_t> mapFirst;          // +0
    rw::Field<uint32_t> mapBegin;          // +4
    rw::Field<uint32_t> mapEnd;            // +8
    rw::Field<uint32_t> mapCap;            // +12
    rw::Field<uint32_t> start;             // +16
    rw::Field<uint32_t> size;              // +20
    // front() of a deque of 4-byte elements
    uint32_t front() const {
        uint32_t block = rw::load<uint32_t>(mapBegin + ((start >> 8) & 0xFFFFFCu));
        return rw::load<uint32_t>(block + (start & 0x3FFu) * 4);
    }
};
RW_SIZE(StdDeque, 24);

struct FlashNode {
    enum BadStatus : int32_t { POISON = 0, SLEEP = 1, PARALYSIS = 2, DEEP_POISON = 3, NONE = 4 };
    enum BadStatusBit : uint32_t { BIT_POISON = 1, BIT_SLEEP = 2, BIT_PARALYSIS = 4, BIT_DEEP_POISON = 8 };

    rw::Field<uint32_t> vtable;            // +0    cocos2d::Node
    const uint8_t pad_004[524];                  // +4    cocos2d::Node fields
    StdFunction nodeOnEnter;               // +528  cocos2d::Node std::function (set to scheduleUpdate this)
    const uint8_t pad_224[4];                    // +548
    StdFunction nodeOnExit;                // +552  cocos2d::Node std::function (set to unscheduleUpdate this)
    const uint8_t pad_23c[52];                   // +572  cocos2d::Node fields
    rw::Field<uint8_t> field_270;          // +624  cleared by the constructor
    rw::Field<uint8_t> statusTinted;       // +625  the bad status display (tint / effect) is on
    rw::Field<uint8_t> field_272;          // +626
    const uint8_t pad_273[1];                    // +627
    rw::Field<uint32_t> field_274;         // +628
    rw::Field<float> vec4_278[4];          // +632  cocos2d::Vec4 (constructed by Vec4::Vec4())
    rw::Field<int32_t> shownStatus;        // +648  BadStatus the effect armature currently plays
    rw::Field<int32_t> status;             // +652  BadStatus to display
    rw::Field<uint8_t> freezeOnStatus;     // +656  sleep / paralysis also stop the LWF animation
    const uint8_t pad_291[3];                    // +657
    rw::Field<float> effectYParalysis;     // +660  effect armature y for paralysis
    rw::Field<float> effectY;              // +664  effect armature y for the other statuses
    rw::Field<uint32_t> lwfNode;           // +668  cocos2d::LWFNode* (+992: its LWF::LWF*)
    // +672 CommandParser (552 bytes): the movie script commands, std::function slots 24 bytes apart
    const uint8_t commandParser[552];            // +672
    StdDeque knockbacks;                   // +1224 std::deque<int> knockback values from the movie
    StdDeque actFlags;                     // +1248 std::deque<ActFlag> (1 hit, 2 / 3 knockback, 4, 6 ...)
    rw::Field<uint32_t> pendingSE;         // +1272 audio::ResourceID to play at the next updateSound
    rw::Field<uint32_t> pendingBGM;        // +1276 audio::ResourceID of a BGM to switch to
    rw::Field<uint8_t> soundPending;       // +1280
    const uint8_t pad_501[3];                    // +1281
    bt::Vector hitEffectIds;               // +1284 std::vector<int>
    rw::Field<uint32_t> effectArmature;    // +1296 cocostudio::Armature* ActMapBuffDeBuff_EffectAnim (z 250)
    rw::Field<uint32_t> statusBits;        // +1300 BadStatusBit set
    rw::Field<uint8_t> statusShown;        // +1304 the display matches the status (not set by the constructor)
    const uint8_t pad_519[3];                    // +1305

    // CommandParser slots (offsets of the std::function members)
    static constexpr uint32_t CMD_KNOCKBACK = 672;     // function<void(int)>
    static constexpr uint32_t CMD_HIT = 696;           // function<void()>: ActFlag 1
    static constexpr uint32_t CMD_SOUND = 720;         // function<void()>: play the pending sounds
    static constexpr uint32_t CMD_SE = 768;            // function<void(const char*)>: sound effect name / id
    static constexpr uint32_t CMD_BGM = 792;           // function<void(const char*)>: BGM name / id
    static constexpr uint32_t CMD_FLAG4 = 816;         // function<void()>: ActFlag 4
    static constexpr uint32_t CMD_MOTION_END = 960;    // function<void()> set by Enemy / Npc
    static constexpr uint32_t CMD_FLAG6 = 1008;        // function<void()>: ActFlag 6

    static constexpr uint32_t VTABLE = 0xF8EDE8;       // _ZTV9FlashNode + 8
    static constexpr uint32_t SIZE = 1308;             // Npc 0x538, Enemy 0x540, Avatar 0x578 derive from it
    static constexpr uint32_t VSLOT_UPDATE_SOUND = 156;     // virtual updateSound(float)
    static constexpr uint32_t VSLOT_GET_HIT_EFFECT_ID = 157; // virtual getHitEffectID() const
    // cocos2d::LWFNode
    static constexpr uint32_t LWF_NODE_LWF = 992;      // LWF::LWF*
    static constexpr uint32_t NODE_SET_COLOR = 139;    // setColor(const Color3B&)
    static constexpr uint32_t NODE_SET_OPACITY = 133;  // setOpacity(GLubyte)

    static FlashNode *construct(FlashNode *self, uint32_t noCommands);
    static FlashNode *destruct(FlashNode *self);
    static void destructDelete(FlashNode *self);
    static void set_node_color_multi(FlashNode *self, float r, float g, float b, float a);
    static void set_node_color_add(FlashNode *self, float r, float g, float b, float a);
    static void set_node_color_multi_add(FlashNode *self, float mr, float mg, float mb, float ma, float ar, float ag,
                                         float ab, float aa);
    static void set_bad_status(FlashNode *self, uint32_t status);
    static void set_loops(FlashNode *self);
    static void change_sub(FlashNode *self);
    static void reflection_status(FlashNode *self, uint32_t force);
    static void update(FlashNode *self, float dt);
    static void updateSound(FlashNode *self, float dt);
    static int32_t getHitEffectID(const FlashNode *self);
    static uint32_t UpdateStatusDeepPoison(FlashNode *self, uint32_t on);
    static uint32_t UpdateStatusPoison(FlashNode *self, uint32_t on);
    static uint32_t UpdateStatusSleep(FlashNode *self, uint32_t on);
    static uint32_t UpdateStatusParalysis(FlashNode *self, uint32_t on);
    static uint32_t isPlaying(const FlashNode *self);
    static int32_t knockbackShift(FlashNode *self);
    static int32_t flagShift(FlashNode *self);
    static void pushBackHitEffectID(FlashNode *self, int32_t id);

    // the constructor's command lambdas (each captures this)
    static void cmdSound(uint32_t lambda);
    static void cmdSE(uint32_t lambda, const uint32_t *name);
    static void cmdBGM(uint32_t lambda, const uint32_t *name);
    static void cmdKnockback(uint32_t lambda, const int32_t *value);
    static void cmdHit(uint32_t lambda);
    static void cmdFlag4(uint32_t lambda);
    static void cmdFlag6(uint32_t lambda);
    static void onEnterLambda(uint32_t lambda);
    static void onExitLambda(uint32_t lambda);
    static void reflectionDoneLambda(uint32_t lambda);
};
RW_LAYOUT(FlashNode, nodeOnEnter, 528);
RW_LAYOUT(FlashNode, nodeOnExit, 552);
RW_LAYOUT(FlashNode, field_270, 624);
RW_LAYOUT(FlashNode, vec4_278, 632);
RW_LAYOUT(FlashNode, shownStatus, 648);
RW_LAYOUT(FlashNode, status, 652);
RW_LAYOUT(FlashNode, freezeOnStatus, 656);
RW_LAYOUT(FlashNode, lwfNode, 668);
RW_LAYOUT(FlashNode, commandParser, 672);
RW_LAYOUT(FlashNode, knockbacks, 1224);
RW_LAYOUT(FlashNode, actFlags, 1248);
RW_LAYOUT(FlashNode, pendingSE, 1272);
RW_LAYOUT(FlashNode, soundPending, 1280);
RW_LAYOUT(FlashNode, hitEffectIds, 1284);
RW_LAYOUT(FlashNode, effectArmature, 1296);
RW_LAYOUT(FlashNode, statusBits, 1300);
RW_LAYOUT(FlashNode, statusShown, 1304);
RW_SIZE(FlashNode, 1308);
