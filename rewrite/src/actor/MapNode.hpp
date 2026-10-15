// MapNode: the base of everything that stands on the quest map (avatar and enemy actors, the raid boss, prizes,
// materials, treasures, gate markers). A cocos2d::Node with a "map position" (field coordinates, y down) kept in
// sync with a child node that runs the movement actions: updateMapPosition() applies the child's displacement since
// the last update to the map position. The depth (z) used to sort nodes on the field is the map y, unless locked.
//
// Common definitions for the actor module live here too: cocos2d::Node virtual slots, frequently called library
// functions (as the lifted code calls them, Thumb bit set).
#pragma once

#include "battle/BattleCommon.hpp"

// ------------------------------------------------------------------------------------------- cocos2d::Node
// Virtual slots of cocos2d::Node (vtable word index; byte offset = slot * 4), from the MapNode vtable.
namespace node_slot {
constexpr uint32_t DTOR = 0;                    // ~Node (complete)
constexpr uint32_t DELETING_DTOR = 1;           // ~Node + operator delete
constexpr uint32_t SET_LOCAL_Z_ORDER = 3;       // setLocalZOrder(int)
constexpr uint32_t SET_SCALE_X = 10;            // setScaleX(float)
constexpr uint32_t GET_SCALE_X = 11;
constexpr uint32_t SET_SCALE_Y = 12;
constexpr uint32_t GET_SCALE_Y = 13;
constexpr uint32_t SET_SCALE = 16;              // setScale(float)
constexpr uint32_t GET_SCALE = 17;
constexpr uint32_t SET_SCALE_XY = 18;           // setScale(float, float)
constexpr uint32_t SET_POSITION = 19;           // setPosition(const Vec2&)
constexpr uint32_t GET_POSITION = 21;           // const Vec2& getPosition() const
constexpr uint32_t SET_POSITION_XY = 23;        // setPosition(float, float)
constexpr uint32_t SET_POSITION_X = 25;
constexpr uint32_t GET_POSITION_X = 26;
constexpr uint32_t SET_POSITION_Y = 27;
constexpr uint32_t GET_POSITION_Y = 28;
constexpr uint32_t SET_ANCHOR_POINT = 39;       // setAnchorPoint(const Vec2&)
constexpr uint32_t GET_ANCHOR_POINT = 40;
constexpr uint32_t SET_CONTENT_SIZE = 42;
constexpr uint32_t GET_CONTENT_SIZE = 43;       // const Size& getContentSize() const
constexpr uint32_t SET_VISIBLE = 44;            // setVisible(bool)
constexpr uint32_t IS_VISIBLE = 45;
constexpr uint32_t SET_ROTATION = 46;
constexpr uint32_t ADD_CHILD = 60;              // addChild(Node*)
constexpr uint32_t ADD_CHILD_Z = 61;            // addChild(Node*, int z)
constexpr uint32_t ADD_CHILD_Z_TAG = 62;        // addChild(Node*, int z, int tag)
constexpr uint32_t ADD_CHILD_Z_NAME = 63;       // addChild(Node*, int z, const std::string&)
constexpr uint32_t GET_CHILD_BY_TAG = 64;
constexpr uint32_t GET_CHILD_BY_NAME = 65;
constexpr uint32_t GET_CHILDREN = 67;
constexpr uint32_t GET_PARENT = 71;
constexpr uint32_t REMOVE_FROM_PARENT = 73;
constexpr uint32_t REMOVE_FROM_PARENT_CLEANUP = 74;  // removeFromParentAndCleanup(bool)
constexpr uint32_t REMOVE_CHILD = 75;           // removeChild(Node*, bool cleanup)
constexpr uint32_t REMOVE_ALL_CHILDREN = 78;
constexpr uint32_t REMOVE_ALL_CHILDREN_CLEANUP = 79;
constexpr uint32_t GET_TAG = 82;
constexpr uint32_t SET_TAG = 83;
constexpr uint32_t ON_ENTER = 93;
constexpr uint32_t ON_EXIT = 95;
constexpr uint32_t CLEANUP = 97;
constexpr uint32_t UPDATE = 113;                // update(float)
constexpr uint32_t GET_OPACITY = 131;
constexpr uint32_t SET_OPACITY = 133;           // setOpacity(GLubyte)
constexpr uint32_t SET_CASCADE_OPACITY = 136;
constexpr uint32_t SET_COLOR = 139;             // setColor(const Color3B&)
constexpr uint32_t SET_CASCADE_COLOR = 142;
constexpr uint32_t SET_COLOR_EFFECT = 145;      // setColorEffect(8 floats) (engine extension)
constexpr uint32_t INIT = 146;                  // bool init()
}  // namespace node_slot

// cocostudio::Armature (a Node) / ArmatureAnimation virtual slots
namespace cs_slot {
constexpr uint32_t ARMATURE_INIT_NAME = 156;       // Armature::init(const std::string&)
constexpr uint32_t ARMATURE_GET_BONE = 159;        // Armature::getBone(const std::string&) const
constexpr uint32_t ARMATURE_GET_ANIMATION = 166;   // Armature::getAnimation() const
constexpr uint32_t ANIM_PAUSE = 3;                 // ArmatureAnimation::pause()
constexpr uint32_t ANIM_RESUME = 4;
constexpr uint32_t ANIM_STOP = 5;
constexpr uint32_t ANIM_GET_CURRENT_FRAME_INDEX = 7;
constexpr uint32_t ANIM_IS_PAUSE = 11;
constexpr uint32_t ANIM_IS_COMPLETE = 13;
constexpr uint32_t ANIM_IS_PLAYING = 15;
constexpr uint32_t ANIM_SET_SPEED_SCALE = 23;
constexpr uint32_t ANIM_PLAY = 26;                 // play(const std::string& name, int durationTo, int loop)
constexpr uint32_t ANIM_PLAY_BY_INDEX = 27;
constexpr uint32_t ANIM_PLAY_WITH_INDEX = 28;      // playWithIndex(int, int durationTo, int loop)
constexpr uint32_t ANIM_GOTO_AND_PLAY = 31;
constexpr uint32_t ANIM_GOTO_AND_PAUSE = 32;       // gotoAndPause(int frame)
}  // namespace cs_slot

namespace actor_fn {
constexpr uint32_t armature_create = 0x79F3C7;            // cocostudio::Armature::create(const std::string&)
constexpr uint32_t adm_getInstance = 0x7AC9B5;            // cocostudio::ArmatureDataManager::getInstance()
constexpr uint32_t adm_addArmatureFileInfo = 0x7AD7BD;    // ArmatureDataManager::addArmatureFileInfo(const std::string&)
constexpr uint32_t anim_setMovementEventCallFunc = 0x7A3C69;  // ArmatureAnimation::setMovementEventCallFunc(std::function)
constexpr uint32_t anim_setFrameEventCallFunc = 0x7A3CE5;     // ArmatureAnimation::setFrameEventCallFunc(std::function)
constexpr uint32_t audio_getInstance = 0x5BECFD;          // audio::Manager::getInstance()
constexpr uint32_t audio_load = 0x5BEEF5;                 // audio::Manager::load(ResourceID)
constexpr uint32_t audio_getLayer = 0x5BF33D;             // audio::Manager::getLayer(int) (sret shared_ptr<Layer>)
constexpr uint32_t layer_playSE = 0x5BF785;               // audio::Layer::playSE(ResourceID, int, int)
constexpr uint32_t strlen_ = 0x40E08C;                    // strlen (import)
constexpr uint32_t node_ctor = 0x892531;                  // cocos2d::Node::Node()
constexpr uint32_t node_dtor = 0x8927ED;                  // cocos2d::Node::~Node()
constexpr uint32_t node_create = 0x89372D;                // cocos2d::Node::create()
constexpr uint32_t node_runAction = 0x894AED;             // cocos2d::Node::runAction(Action*)
constexpr uint32_t node_stopAction = 0x894B1D;            // cocos2d::Node::stopAction(Action*)
constexpr uint32_t node_getNumberOfRunningActions = 0x894B4F;  // cocos2d::Node::getNumberOfRunningActions() const
constexpr uint32_t node_pause = 0x894A4F;                 // cocos2d::Node::pause()
constexpr uint32_t node_resume = 0x8948C9;                // cocos2d::Node::resume()
constexpr uint32_t node_removeAllChildrenWithCleanup = 0x894369;  // Node::removeAllChildrenWithCleanup(bool)
constexpr uint32_t ref_retain = 0x8FD0B5;                 // cocos2d::Ref::retain()
constexpr uint32_t ref_release = 0x8FD0BD;                // cocos2d::Ref::release()
constexpr uint32_t ref_autorelease = 0x8FD0D5;            // cocos2d::Ref::autorelease()
constexpr uint32_t vec2_ZERO = 0x1006950;                 // cocos2d::Vec2::ZERO (8 bytes)
constexpr uint32_t aeabi_memset = 0x40E7A0;               // __aeabi_memset(dst, n, c)
constexpr uint32_t string_init = 0x47CD93;                // std::string::__init(const char*, size_t) (sub_47CD92)
}  // namespace actor_fn

// ------------------------------------------------------------------------------------------- MapNode
struct MapNode {
    rw::Field<uint32_t> vtable;            // +0   cocos2d::Node
    const uint8_t pad_004[620];                  // +4   cocos2d::Node fields
    bt::Vec2 mapPosition;                  // +624 position on the field map
    rw::Field<uint32_t> moveNode;          // +632 cocos2d::Node* child that runs the movement actions
    bt::Vec2 moveNodeLast;                 // +636 the move node's position at the last updateMapPosition
    rw::Field<float> lockedZ;              // +644 depth override; 2.0 = unlocked (depth = map y)

    static constexpr uint32_t VTABLE = 0xF91D04;   // _ZTV7MapNode + 8
    static constexpr uint32_t NODE_SIZE = 624;     // sizeof(cocos2d::Node)
    static constexpr uint32_t SIZE = 648;
    static constexpr float UNLOCKED_Z = 2.0f;

    static MapNode *construct(MapNode *self);
    static MapNode *destruct(MapNode *self);
    static void destructDelete(MapNode *self);
    static uint32_t init(MapNode *self);
    static uint32_t move(MapNode *self, uint32_t action);
    static uint32_t updateMapPosition(MapNode *self);
    static float getMapPosZ(MapNode *self);
    static MapNode *lockMapPosZ(MapNode *self, float z);
    static MapNode *unlockMapPosZ(MapNode *self);
    static uint32_t pause(MapNode *self);
    static uint32_t resume(MapNode *self);
};
RW_LAYOUT(MapNode, mapPosition, 624);
RW_LAYOUT(MapNode, moveNode, 632);
RW_LAYOUT(MapNode, moveNodeLast, 636);
RW_LAYOUT(MapNode, lockedZ, 644);
RW_SIZE(MapNode, 648);

// ------------------------------------------------------------------------------------------- helpers
namespace actor {
// A std::string temporary on the guest stack (the original's `std::string(literal)` argument): built with
// std::string::__init(ptr, len) at guest address `at`, freed with freeString (operator delete of a long string).
inline void makeString(uint32_t at, uint32_t chars, uint32_t len) { rw::call(actor_fn::string_init, {at, chars, len}); }
inline void freeString(uint32_t at) {
    if (rw::load<uint8_t>(at) & 1) rw::call(bt::fn::operator_delete, {rw::load<uint32_t>(at + 8)});
}
// Virtual call on a guest object given by address.
inline uint32_t vcallAt(uint32_t obj, uint32_t slot, std::initializer_list<uint32_t> args) {
    return rw::vcall(rw::host_ptr<void>(obj), slot, args);
}
inline uint32_t ld(uint32_t a) { return rw::load<uint32_t>(a); }
inline int32_t ldi(uint32_t a) { return rw::load<int32_t>(a); }
inline float ldf(uint32_t a) { return rw::load<float>(a); }
inline uint8_t ld8(uint32_t a) { return rw::load<uint8_t>(a); }
inline uint32_t fb(float f) { return rw::fbits(f); }
}  // namespace actor
