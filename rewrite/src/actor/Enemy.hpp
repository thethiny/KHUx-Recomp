// Enemy: the LWF character of an enemy (lwf/character/enemy/<id>/<motion>/<motion>.lwf), a FlashNode. Each motion
// is its own LWF file, so alterMotion() replaces the LWF node; the first "wait" load records the attack / centre
// / battle-shift points of the movie ("all" > "body" > "attack" / "center").
// Npc (Npc.hpp) is the same for NPCs with a single motion. Both are driven by EnemyActor / MapNonPlayableCharacter.
#pragma once

#include "FlashNode.hpp"

struct Enemy : FlashNode {
    enum Motion : int32_t { WAIT = 0, BWAIT = 1, MOVE = 2, ATTACK = 3, ATTACK_SP = 4, KNOCKBACK = 5, DEAD = 6 };

    rw::Field<int32_t> enemyId;            // +1308 enemy (character) id: the LWF folder
    rw::Field<int32_t> motion;             // +1312 Motion loaded (7 before the first load)
    rw::Field<uint8_t> firstLoad;          // +1316 the next wait load records the movie points
    rw::Field<uint8_t> battleShiftSet;     // +1317 the battle shift was corrected by the first bwait load
    rw::Field<uint8_t> hasBwait;           // +1318 bwait/bwait.lwf exists
    rw::Field<uint8_t> hasAttackSp;        // +1319 atkSp/atkSp.lwf exists
    bt::Vec2 attackPoint;                  // +1320 "attack" movie position (y flipped)
    bt::Vec2 centerPoint;                  // +1328 "center" movie position
    bt::Vec2 battleShift;                  // +1336 "all" position (wait) minus "all" position (bwait)

    static constexpr uint32_t VTABLE = 0xF8F8B0;   // _ZTV5Enemy + 8
    static constexpr uint32_t SIZE = 0x540;

    static Enemy *create(int32_t enemyId);
    static Enemy *construct(Enemy *self, int32_t enemyId);
    static void alterMotion(Enemy *self, int32_t motion);
    static void preloadResources(const Enemy *self);
    static int32_t getHitEffectID(const Enemy *self);
    static uint32_t getBattleShift(const Enemy *self);
    static Enemy *destruct(Enemy *self);
    static void destructDelete(Enemy *self);
    static void motionEndLambda(uint32_t lambda);
};
RW_LAYOUT(Enemy, enemyId, 1308);
RW_LAYOUT(Enemy, motion, 1312);
RW_LAYOUT(Enemy, firstLoad, 1316);
RW_LAYOUT(Enemy, attackPoint, 1320);
RW_LAYOUT(Enemy, battleShift, 1336);
RW_SIZE(Enemy, 0x540);

// Shared by Enemy and Npc: load an LWF motion file into a fresh LWF node.
namespace flashlwf {
constexpr uint32_t lwfNode_create = rwlib::cocos2d::LWFNode::create;        // cocos2d::LWFNode::create(const char*, void*)
constexpr uint32_t stringBuffer_fset = 0x491501;     // StringBuffer::fset(const char* fmt, ...)
constexpr uint32_t lwfUtil_preload = 0x491361;       // LWFUtil::preloadResources(const std::string&)
constexpr uint32_t lwfHandler_assign = 0x5C9BE5;     // std::function<void(const string&, LWF::Movie*, LWF::Button*)>::operator=
constexpr uint32_t commandParserBind = 0xF8F52C;     // __func vtable of std::bind(&CommandParser::parse, parser, _1, _2, _3)
constexpr uint32_t commandParser_parse = 0x4F28F9;   // CommandParser::parse(const string&, LWF::Movie*, LWF::Button*)
constexpr uint32_t lwf_searchMovieInstance = rwlib::LWF::LWF::SearchMovieInstance;   // LWF::LWF::SearchMovieInstance(const string&) const
constexpr uint32_t movie_searchMovieInstance = rwlib::LWF::Movie::SearchMovieInstance; // LWF::Movie::SearchMovieInstance(const string&, bool) const
constexpr uint32_t movie_gotoAndPlay = rwlib::LWF::Movie::GotoAndPlay__RKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE;     // LWF::Movie::GotoAndPlay(const string&)
constexpr uint32_t movie_getX = rwlib::LWF::Movie::GetX;            // LWF::Movie::GetX() const
constexpr uint32_t movie_getY = rwlib::LWF::Movie::GetY;            // LWF::Movie::GetY() const
constexpr uint32_t LWF_HANDLER = 248;                // LWF::LWF: script command handler (std::function)
constexpr uint32_t MOVIE_FRAME = 156;                // LWF::Movie: current frame (1 = not started)
constexpr uint32_t NODE_REMOVE_CHILD = 75;           // cocos2d::Node::removeChild(Node*, bool)
constexpr uint32_t NODE_UPDATE = 113;                // cocos2d::Node::update(float)
constexpr uint32_t STR_CHAR = 0xDFCDC1;              // "char"
constexpr uint32_t STR_START = 0xE01812;             // "start"
}  // namespace flashlwf
