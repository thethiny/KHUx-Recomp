#include "StageFieldManager.hpp"

namespace {

using bt::bits;
using bt::fromBits;

// ------------------------------------------------------------------------------------------- guest addresses
namespace a {
// cocos2d
constexpr uint32_t ref_ctor = rwlib::cocos2d::Ref::ctor_base;               // cocos2d::Ref::Ref()
constexpr uint32_t ref_retain = rwlib::cocos2d::Ref::retain;             // cocos2d::Ref::retain()
constexpr uint32_t ref_release = rwlib::cocos2d::Ref::release;            // cocos2d::Ref::release()
constexpr uint32_t aeabi_memset = 0x40E7A0;           // __aeabi_memset(dst, n, c)
constexpr uint32_t vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__v;              // cocos2d::Vec2::Vec2()
constexpr uint32_t vec2_ctor_ff = rwlib::cocos2d::Vec2::ctor_base__ff;           // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;              // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;              // cocos2d::Vec2::~Vec2()
constexpr uint32_t vec2_set = rwlib::cocos2d::Vec2::set__ff;               // cocos2d::Vec2::set(float, float)
constexpr uint32_t vec2_add = rwlib::cocos2d::Vec2::add;               // cocos2d::Vec2::add(const Vec2&)
constexpr uint32_t vec2_subtract = rwlib::cocos2d::Vec2::subtract;          // cocos2d::Vec2::subtract(const Vec2&)
constexpr uint32_t vec2_scale = rwlib::cocos2d::Vec2::scale__f;             // cocos2d::Vec2::scale(float)
constexpr uint32_t vec2_distance = rwlib::cocos2d::Vec2::distance;          // cocos2d::Vec2::distance(const Vec2&) const
constexpr uint32_t vec2_distanceSquared = rwlib::cocos2d::Vec2::distanceSquared;   // cocos2d::Vec2::distanceSquared(const Vec2&) const
constexpr uint32_t vec2_isZero = rwlib::cocos2d::Vec2::isZero;            // cocos2d::Vec2::isZero() const
constexpr uint32_t vec2_normalize = rwlib::cocos2d::Vec2::normalize;         // cocos2d::Vec2::normalize()
constexpr uint32_t vec2_dot = rwlib::cocos2d::Vec2::dot;               // cocos2d::Vec2::dot(const Vec2&, const Vec2&)
constexpr uint32_t vec2_ZERO = 0x1006950;             // cocos2d::Vec2::ZERO
constexpr uint32_t size_ctor_ff = rwlib::cocos2d::Size::ctor_base__ff;           // cocos2d::Size::Size(float, float)
constexpr uint32_t rect_ctor = rwlib::cocos2d::Rect::ctor_base__v;              // cocos2d::Rect::Rect()
constexpr uint32_t rect_ctor4 = rwlib::cocos2d::Rect::ctor_base__ffff;             // cocos2d::Rect::Rect(float, float, float, float)
constexpr uint32_t rect_copy = rwlib::cocos2d::Rect::ctor_base__RKS0_;              // cocos2d::Rect::Rect(const Rect&)
constexpr uint32_t rect_setRect = rwlib::cocos2d::Rect::setRect;           // cocos2d::Rect::setRect(float, float, float, float)
constexpr uint32_t rect_getMinX = rwlib::cocos2d::Rect::getMinX;           // cocos2d::Rect::getMinX() const
constexpr uint32_t rect_getMaxX = rwlib::cocos2d::Rect::getMaxX;           // cocos2d::Rect::getMaxX() const
constexpr uint32_t rect_getMinY = rwlib::cocos2d::Rect::getMinY;           // cocos2d::Rect::getMinY() const
constexpr uint32_t rect_getMaxY = rwlib::cocos2d::Rect::getMaxY;           // cocos2d::Rect::getMaxY() const
constexpr uint32_t director_getInstance = rwlib::cocos2d::Director::getInstance;   // cocos2d::Director::getInstance()
constexpr uint32_t director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;    // cocos2d::Director::getVisibleSize() const
constexpr uint32_t director_getVisibleOrigin = rwlib::cocos2d::Director::getVisibleOrigin;  // cocos2d::Director::getVisibleOrigin() const
constexpr uint32_t node_create = rwlib::cocos2d::Node::create;            // cocos2d::Node::create()
constexpr uint32_t node_runAction = rwlib::cocos2d::Node::runAction;         // cocos2d::Node::runAction(Action*)
constexpr uint32_t node_convertToWorldSpace = rwlib::cocos2d::Node::convertToWorldSpace;   // cocos2d::Node::convertToWorldSpace(const Vec2&) const
constexpr uint32_t data_getBytes = rwlib::cocos2d::Data::getBytes;          // cocos2d::Data::getBytes() const
constexpr uint32_t data_getSize = rwlib::cocos2d::Data::getSize;           // cocos2d::Data::getSize() const
constexpr uint32_t scaleTo_create = rwlib::cocos2d::ScaleTo::create__ff;         // cocos2d::ScaleTo::create(float, float)
constexpr uint32_t easeSineInOut_create = rwlib::cocos2d::EaseSineInOut::create;   // cocos2d::EaseSineInOut::create(ActionInterval*)
constexpr uint32_t bezierBy_create = rwlib::cocos2d::BezierBy::create;        // cocos2d::BezierBy::create(float, const _ccBezierConfig&)
constexpr uint32_t lwfNode_create = rwlib::cocos2d::LWFNode::create;         // cocos2d::LWFNode::create(const char*, void*)
constexpr uint32_t roundf_ = 0x419C54;                // roundf (PLT)
constexpr uint32_t powf_ = 0x41ABFC;                  // powf (PLT)
constexpr uint32_t add_shared = 0xC8DBB5;             // std::__shared_count::__add_shared()
// cocos2d::Vector<T*> instantiations
constexpr uint32_t vecMapNode_dtor = 0x5DA621, vecMapNode_clear = 0x5DAAB9, vecMapNode_pushBack = 0x5DAEC1,
                   vecMapNode_erase = 0x5DAF5D;
constexpr uint32_t vecPrizes_dtor = 0x5DB649, vecPrizes_clear = 0x5DB615, vecPrizes_pushBack = 0x5DB985,
                   vecPrizes_erase = 0x60A53F, vecPrizes_copy = 0x60A4ED;
constexpr uint32_t vecMaterial_dtor = 0x5FB655, vecMaterial_clear = 0x5FB70B, vecMaterial_pushBack = 0x755745,
                   vecMaterial_erase = 0x7559A5;
constexpr uint32_t vecTreasure_dtor = 0x5FB629, vecTreasure_clear = 0x5FB769, vecTreasure_pushBack = 0x7557AD,
                   vecTreasure_erase = 0x755A77;
constexpr uint32_t vecNpc_dtor = 0x49608B, vecNpc_clear = 0x496653, vecNpc_pushBack = 0x755815,
                   vecNpc_erase = 0x755B49;
constexpr uint32_t vecGate_dtor = 0x5FB5FD, vecGate_clear = 0x5FB7C7, vecGate_pushBack = 0x75587D,
                   vecGate_erase = 0x755C1B;
constexpr uint32_t vecLayer_dtor = 0x5FB681, vecLayer_clear = 0x5FB6AD, vecLayer_pushBack = 0x7554A1;
constexpr uint32_t vecNode_copy = 0x4962C5, vecNode_dtor = 0x496317;   // cocos2d::Vector<cocos2d::Node*>
// std containers
constexpr uint32_t layerMap_at = 0x755429;            // unordered_map<int, MapFieldLayer*>::at
constexpr uint32_t jumpMap_find = 0x758511;           // __hash_table<int, vector<MapJumpInfo*>*>::find<int>
constexpr uint32_t jumpMap_at = 0x756DB5;             // unordered_map<int, vector<MapJumpInfo*>*>::at
// game
constexpr uint32_t stageManager_getMapStartInfo = 0x75BBD9;   // StageManager::getMapStartInfo()
constexpr uint32_t mapFieldLayer_create = 0x5DA985;           // MapFieldLayer::create(const Info*)
constexpr uint32_t mapFieldLayer_remove = 0x5DAA7D;           // MapFieldLayer::remove()
constexpr uint32_t mapFieldLayer_update = 0x5DAAED;           // MapFieldLayer::update(float, float)
constexpr uint32_t mapFieldLayer_addFieldMapNode = 0x5DAE91;  // MapFieldLayer::addFieldMapNode(MapNode*, int)
constexpr uint32_t mapFieldLayer_addLayerMapNode = 0x5DB031;  // MapFieldLayer::addLayerMapNode(MapNode*, int)
constexpr uint32_t mapFieldLayer_removeFieldMapNode = 0x5DAF29;   // MapFieldLayer::removeFieldMapNode(MapNode*)
constexpr uint32_t mapNode_unlockMapPosZ = 0x5DA381;          // MapNode::unlockMapPosZ()
constexpr uint32_t mapNode_getMapPosZ = 0x5DA54D;             // MapNode::getMapPosZ()
constexpr uint32_t mapNode_move = 0x5DA415;                   // MapNode::move(cocos2d::Action*)
constexpr uint32_t snowParticle_create = 0x5ED0BD;           // SnowParticle::create(Style)
constexpr uint32_t snowParticle_initCoordinate = 0x5ED119;   // SnowParticle::initCoordinate(Vec2&)
constexpr uint32_t snowParticle_updateCoordinate = 0x5ED629; // SnowParticle::updateCoordinate(Vec2&, float)
constexpr uint32_t npc_showTalkicon = 0x5DC287;   // MapNonPlayableCharacter::show_talkicon(bool)
constexpr uint32_t npc_getTapCenterMapPosition = 0x5DC4C1;   // MapNonPlayableCharacter::getTapCenterMapPosition()
constexpr uint32_t npc_getTapRange = 0x5DC523;    // MapNonPlayableCharacter::getTapRange()
constexpr uint32_t npc_lookAt = 0x5DC491;         // MapNonPlayableCharacter::lookAt(Vec2&)
constexpr uint32_t npc_talk = 0x5DC3A5;           // MapNonPlayableCharacter::talk(StageManager*)
constexpr uint32_t mapPrize_breakAwayArmature = 0x5DD399;    // MapPrize::breakAwayArmature()
constexpr uint32_t mapPrize_getItem = 0x5DCE95;              // MapPrize::getItem(Item*, bool)
constexpr uint32_t mapPrize_setAbsorptionAttract = 0x5DD1DD; // MapPrize::setAbsorptionAttract(Vec2&, function, bool)
constexpr uint32_t mapPrizes_isFinishEruption = 0x5DEBA5;    // MapPrizes::isFinishEruption()
constexpr uint32_t mapMaterial_popup = 0x5DBB65;             // MapMaterial::popup()
constexpr uint32_t mapTreasure_open = 0x5DF6A9;              // MapTreasure::open(std::function<void(MapTreasure*)>)
constexpr uint32_t mapTreasure_terminate = 0x5DF5C7;         // MapTreasure::terminate()
constexpr uint32_t avatarActor_addPrize = 0x740CF5;          // AvatarActor::addPrize(MapPrize*)
constexpr uint32_t avatarActor_delPrize = 0x740E09;          // AvatarActor::delPrize(MapPrize*, bool)
constexpr uint32_t avatarActor_clearPrize = 0x741035;        // AvatarActor::clearPrize()
constexpr uint32_t avatarActor_lookAt = 0x741651;            // AvatarActor::lookAt(const Vec2&)
constexpr uint32_t avatar_changeMotion = 0x5C63C5;           // Avatar::changeMotion(Motion)
constexpr uint32_t appState_getInstance = 0x4E66DD;          // AppState::getInstance()
constexpr uint32_t stageState_addMaterial = 0x4EF141;        // StageState::addMaterial(unsigned, unsigned)
constexpr uint32_t stageState_addTreasure = 0x4EF341;        // StageState::addTreasure(unsigned, unsigned)
constexpr uint32_t stageState_addActionAlreadyNPC = 0x4EF3F1;    // StageState::addActionAlreadyNPC(unsigned)
constexpr uint32_t appGetFrameWidth = 0x799B31;              // AppGetGrameFrameWidth()
constexpr uint32_t appGetFrameHeight = 0x799D21;             // AppGetGrameFrameHeight()
constexpr uint32_t hitShapeGroupInfo_isHitTest = 0x757D21;   // HitShapeGroupInfo::isHitTest(Rect&, Rect&) const
constexpr uint32_t hitShapeInfo_isIntersects = 0x758009;     // HitShapeInfo::isIntersects(Vec2&, Rect&) const
constexpr uint32_t enemyActor_getMapPosotionTouchRect = 0x74C849;  // EnemyActor::getMapPosotionTouchRect()
// data
constexpr uint32_t holeRainLwf = 0xE073B0;            // "lwf/map/hole_rain/hole_rain.lwf"
constexpr uint32_t openTreasureLambdaVtable = 0xF6E1E8;   // std::__function::__func vtable (onEventOccursTreasure lambda)
constexpr uint32_t attractLambdaVtable = 0xF6E1BC;        // std::__function::__func vtable (addPrizeAttract lambda)
constexpr uint32_t collisionDirections = 0xE9EC90;    // int[6][2] step per direction (isCollisionRectSafe)
constexpr uint32_t collisionSideChecks = 0xE9ECC0;    // int[6][2] side probe offsets (directions 2..5)
constexpr uint32_t stateBoardGroups = 0xF6E174;       // HitShapeGroupInfo[4] (16 bytes; float step at +8)
constexpr uint32_t touchUiShapes = 0x1004090;         // HitShapeInfo[3] (20 bytes each)
constexpr uint32_t touchUiShapeLast = 0x1004074;      // HitShapeInfo (checked with the rect's max-x/min-y corner)
}  // namespace a

// Vtable slots (byte offset / 4) used on guest objects.
constexpr uint32_t SLOT_DELETE = 1;               // deleting destructor
constexpr uint32_t SLOT_INIT = 2;                 // init()
constexpr uint32_t SLOT_NODE_SET_LOCAL_Z = 4;     // MapNode +0x10: setLocalZOrder(int)
constexpr uint32_t SLOT_GET_SCALE = 17;           // Node::getScale() (+0x44)
constexpr uint32_t SLOT_SET_POSITION = 19;        // Node::setPosition(const Vec2&) (+0x4C)
constexpr uint32_t SLOT_GET_POSITION = 21;        // Node::getPosition() (+0x54)
constexpr uint32_t SLOT_SET_POSITION_XY = 23;     // Node::setPosition(float, float) (+0x5C)
constexpr uint32_t SLOT_ADD_CHILD = 60;           // Node::addChild(Node*) (+0xF0)
constexpr uint32_t SLOT_ADD_CHILD_Z = 61;         // Node::addChild(Node*, int) (+0xF4)
constexpr uint32_t SLOT_GET_CHILDREN = 67;        // Node::getChildren() (+0x10C)
constexpr uint32_t SLOT_REMOVE_FROM_PARENT = 73;  // Node::removeFromParent() (+0x124)
constexpr uint32_t SLOT_REMOVE_ALL_CHILDREN = 78; // Node::removeAllChildren() (+0x138)
constexpr uint32_t SLOT_MAPNODE_VISIBLE = 45;     // MapNode +0xB4: is shown (1) (sorted by depth)
constexpr uint32_t SLOT_GATE_SET_ENABLE = 44;     // MapGateMarker +0xB0
constexpr uint32_t SLOT_MAPNODE_KIND = 82;        // MapNode +0x148: node kind

// MapFieldLayer / MapNode / actor offsets
constexpr uint32_t LAYER_WIDTH = 0x6B0, LAYER_HEIGHT = 0x6B4, LAYER_ID = 0x6C0;
constexpr uint32_t NODE_POS = 0x270;              // MapNode map position (Vec2)
constexpr uint32_t NODE_SAFEBUFFER = 0x288;       // MapMaterial / MapTreasure / NPC: SafeBuffer with id (0) / count (4)
constexpr uint32_t ACTOR_AVATAR = 0x288;          // AvatarActor: Avatar*
constexpr uint32_t PRIZES_LIST_PLAYER = 0x294;    // MapPrizes: prizes flying to the player (begin, end)
constexpr uint32_t PRIZES_LIST_ATTRACT = 0x2A0;   // MapPrizes: prizes flying to a HUD target
constexpr uint32_t PRIZES_LIST_MIXED = 0x2AC;     // MapPrizes: prizes added or dropped by breakAwayArmature
constexpr uint32_t PRIZE_READY = 0x2B0;           // MapPrize byte: may break away
constexpr uint32_t CHEST_OPENED = 0x2B8;          // MapMaterial byte / MapTreasure word: already taken
constexpr uint32_t MATERIAL_PRIZES = 0x2AC;       // MapMaterial / MapTreasure: Vector<MapPrizes*>

inline uint32_t ld(uint32_t addr) { return rw::load<uint32_t>(addr); }
inline int32_t ldi(uint32_t addr) { return rw::load<int32_t>(addr); }
inline float ldf(uint32_t addr) { return rw::load<float>(addr); }
inline uint32_t ga(const void *p) { return rw::guest_addr(p); }
inline uint32_t vc(uint32_t obj, uint32_t slot, std::initializer_list<uint32_t> args) {
    return rw::vcall(rw::host_ptr<void>(obj), slot, args);
}
inline float callF(uint32_t fn, std::initializer_list<uint32_t> args) { return fromBits(rw::call(fn, args)); }
inline float rectF(uint32_t getter, uint32_t rect) { return callF(getter, {rect}); }
inline float roundF(float v) { return callF(a::roundf_, {bits(v)}); }
inline uint32_t appState() { return rw::call(a::appState_getInstance, {}); }

}  // namespace

// ============================================================================================ construction
StageFieldManager *StageFieldManager::create() {
    uint32_t obj = rw::call(bt::fn::operator_new, {SIZE});
    StageFieldManager *self = rw::host_ptr<StageFieldManager>(obj);
    construct(self);
    if (vc(obj, SLOT_INIT, {obj}) == 1) {
        rw::call(bt::fn::ref_autorelease, {obj});
        return self;
    }
    vc(obj, SLOT_DELETE, {obj});
    return nullptr;
}
RW_FUNCTION(0x5FA260, "StageFieldManager::create", StageFieldManager::create);

StageFieldManager *StageFieldManager::construct(StageFieldManager *self) {
    uint32_t s = ga(self);
    rw::call(a::ref_ctor, {s});
    self->vtable = VTABLE;
    rw::call(a::aeabi_memset, {s + 0x14, 0x14, 0});   // fieldNode .. layers
    rw::call(a::aeabi_memset, {s + 0x3C, 0x48, 0});   // the six node vectors
    rw::call(a::vec2_ctor, {ga(&self->screenCenter)});
    rw::call(a::vec2_ctor, {ga(&self->cameraPos)});
    rw::call(a::rect_ctor, {ga(&self->cameraBounds)});
    rw::call(a::rect_ctor, {ga(&self->mapRect)});
    rw::set<uint32_t>(self, 0xBC, 0);   // materialRange
    self->eventType = 0;
    rw::call(a::vec2_ctor, {ga(&self->eventPos)});
    self->talkingNpc = 0;
    self->luxTarget.node = 0;
    rw::call(a::vec2_copy, {ga(&self->luxTarget.offset), a::vec2_ZERO});
    self->flaggedItemTarget.node = 0;
    rw::call(a::vec2_copy, {ga(&self->flaggedItemTarget.offset), a::vec2_ZERO});
    self->itemTarget.node = 0;
    rw::call(a::vec2_copy, {ga(&self->itemTarget.offset), a::vec2_ZERO});
    self->weather = 0;
    return self;
}
RW_FUNCTION(0x5FB430, "StageFieldManager::StageFieldManager", StageFieldManager::construct);

StageFieldManager *StageFieldManager::destruct(StageFieldManager *self) {
    self->vtable = VTABLE;
    removeField(self);
    if (self->fieldNode) rw::call(a::ref_release, {self->fieldNode});
    uint32_t s = ga(self);
    for (uint32_t off : {0xF4u, 0xE8u, 0xDCu, 0xC4u, 0xACu, 0x9Cu, 0x94u, 0x8Cu}) rw::call(a::vec2_dtor, {s + off});
    rw::call(a::vecGate_dtor, {ga(&self->gateMarkers)});
    rw::call(a::vecNpc_dtor, {ga(&self->npcs)});
    rw::call(a::vecTreasure_dtor, {ga(&self->treasures)});
    rw::call(a::vecMaterial_dtor, {ga(&self->materials)});
    rw::call(a::vecPrizes_dtor, {ga(&self->prizes)});
    rw::call(a::vecMapNode_dtor, {ga(&self->nodes)});
    rw::call(a::vecLayer_dtor, {ga(&self->layers)});
    rw::call(bt::fn::ref_dtor, {s});
    return self;
}
RW_FUNCTION(0x754C9C, "StageFieldManager::~StageFieldManager", StageFieldManager::destruct);

void StageFieldManager::destructDelete(StageFieldManager *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x754EA4, "StageFieldManager::~StageFieldManager(deleting)", StageFieldManager::destructDelete);

// Empty every node list, remove the map layers and the field node's children; forget the weather node.
void StageFieldManager::removeField(StageFieldManager *self) {
    rw::call(a::vecMapNode_clear, {ga(&self->nodes)});
    rw::call(a::vecPrizes_clear, {ga(&self->prizes)});
    rw::call(a::vecMaterial_clear, {ga(&self->materials)});
    rw::call(a::vecTreasure_clear, {ga(&self->treasures)});
    rw::call(a::vecNpc_clear, {ga(&self->npcs)});
    rw::call(a::vecGate_clear, {ga(&self->gateMarkers)});
    uint32_t end = self->layers.end;
    for (uint32_t p = self->layers.begin; p != end; p += 4) rw::call(a::mapFieldLayer_remove, {ld(p)});
    rw::call(a::vecLayer_clear, {ga(&self->layers)});
    vc(self->fieldNode, SLOT_REMOVE_ALL_CHILDREN, {self->fieldNode});
    vc(self->fieldNode, SLOT_REMOVE_FROM_PARENT, {self->fieldNode});
    if (self->weather) self->weather = 0;
}
RW_FUNCTION(0x754E18, "StageFieldManager::removeField", StageFieldManager::removeField);

uint32_t StageFieldManager::init(StageFieldManager *self) {
    uint32_t node = rw::call(a::node_create, {});
    self->fieldNode = node;
    rw::call(a::ref_retain, {node});
    return 1;
}
RW_FUNCTION(0x754EB0, "StageFieldManager::init", StageFieldManager::init);

// ============================================================================================ field setup
// Raid field: one empty MapFieldLayer the size of the screen.
void StageFieldManager::setStageRaid(StageFieldManager *self, uint32_t layer) {
    // frame: +0x00 visible size, +0x08 visible origin, +0x10 screen centre, +0x18 MapFieldLayer::Info
    //        (Size +0x18, Rect +0x20, ints +0x30/+0x34/+0x38, byte +0x3C)
    bt::GuestFrame f(0x40);
    uint32_t dir = rw::call(a::director_getInstance, {});
    rw::call(a::director_getVisibleSize, {f.addr(0x00), dir});
    dir = rw::call(a::director_getInstance, {});
    rw::call(a::director_getVisibleOrigin, {f.addr(0x08), dir});
    float w = ldf(f.addr(0x00)), h = ldf(f.addr(0x04));
    float cx = w * 0.5f + ldf(f.addr(0x08));
    float cy = h * 0.5f + ldf(f.addr(0x0C));
    rw::call(a::vec2_ctor_ff, {f.addr(0x10), bits(cx), bits(cy)});
    removeField(self);
    rw::call(a::size_ctor_ff, {f.addr(0x18), bits(w), bits(h)});
    rw::call(a::rect_ctor4, {f.addr(0x20), 0, 0, bits(w), bits(h)});
    f.setWord(0x30, 0);
    f.setWord(0x34, 0);
    f.setWord(0x38, 100);
    rw::store8(f.ptr<uint8_t>(0x3C), 0);
    uint32_t ml = rw::call(a::mapFieldLayer_create, {f.addr(0x18)});
    self->mainLayer = ml;
    vc(self->fieldNode, SLOT_ADD_CHILD_Z, {self->fieldNode, ml, 100});
    ml = self->mainLayer;
    w = ldf(f.addr(0x00));
    h = ldf(f.addr(0x04));
    rw::call(a::vec2_set, {ga(&self->screenCenter), bits(w * 0.5f), bits(h * 0.5f)});
    float lw = ldf(ml + LAYER_WIDTH), lh = ldf(ml + LAYER_HEIGHT);
    rw::call(a::rect_setRect, {ga(&self->cameraBounds), bits(w * 0.5f), bits(h * 0.5f), bits(lw - w), bits(lh - h)});
    rw::call(a::rect_setRect, {ga(&self->mapRect), 0, 0, bits(ldf(ml + LAYER_WIDTH)), bits(ldf(ml + LAYER_HEIGHT))});
    vc(self->fieldNode, SLOT_SET_POSITION, {self->fieldNode, f.addr(0x10)});
    vc(layer, SLOT_ADD_CHILD, {layer, self->fieldNode});
    self->zoomState = 0;
    self->zoomRequest = 0;
}
RW_FUNCTION(0x754EC4, "StageFieldManager::setStageRaid", StageFieldManager::setStageRaid);

// Quest field: the stage's map layers and nodes, weather, camera at the start position. Returns the start info.
uint32_t StageFieldManager::setStageMapInfo(StageFieldManager *self, uint32_t layer, uint32_t sm) {
    // frame: +0x00 visible size, +0x08 visible origin, +0x10 screen centre, +0x18 camera position, +0x20 key 0
    bt::GuestFrame f(0x28);
    uint32_t dir = rw::call(a::director_getInstance, {});
    rw::call(a::director_getVisibleSize, {f.addr(0x00), dir});
    dir = rw::call(a::director_getInstance, {});
    rw::call(a::director_getVisibleOrigin, {f.addr(0x08), dir});
    float cx = ldf(f.addr(0x00)) * 0.5f + ldf(f.addr(0x08));
    float cy = ldf(f.addr(0x04)) * 0.5f + ldf(f.addr(0x0C));
    rw::call(a::vec2_ctor_ff, {f.addr(0x10), bits(cx), bits(cy)});
    removeField(self);
    uint32_t start = rw::call(a::stageManager_getMapStartInfo, {sm});
    f.setWord(0x20, 0);
    self->mainLayer = ld(rw::call(a::layerMap_at, {sm + 0x110, f.addr(0x20)}));
    // every MapFieldLayer of the stage (the unordered_map's node list: next +0, value +0xC)
    for (uint32_t node = ld(sm + 0x118); node; node = ld(node)) {
        uint32_t ml = ld(node + 0xC);
        vc(self->fieldNode, SLOT_ADD_CHILD_Z, {self->fieldNode, ml, ld(ml + LAYER_ID)});
        rw::call(a::vecLayer_pushBack, {ga(&self->layers), ml});
    }
    uint32_t end = ld(sm + 0x134);
    for (uint32_t p = ld(sm + 0x130); p != end; p += 4) addMapNodeToField(self, ld(p));
    uint32_t weatherType = 0;
    if (rw::load<uint8_t>(sm + 0x10C)) weatherType = ld(sm + 0x108);
    uint32_t wn = getWeatherNode(self, weatherType, f.ptr<bt::Vec2>(0x00));
    if (wn) vc(self->fieldNode, SLOT_ADD_CHILD_Z, {self->fieldNode, wn, ld(sm + 0x11C) + 1});
    self->jumpInfoMap = sm + 0xF4;
    self->collisionData = sm + 0xE4;
    self->jumpData = sm + 0xEC;
    uint32_t ml = self->mainLayer;
    float w = ldf(f.addr(0x00)), h = ldf(f.addr(0x04));
    rw::call(a::vec2_set, {ga(&self->screenCenter), bits(w * 0.5f), bits(h * 0.5f)});
    rw::call(a::rect_setRect, {ga(&self->cameraBounds), bits(w * 0.5f), bits(h * 0.5f),
                               bits(ldf(ml + LAYER_WIDTH) - w), bits(ldf(ml + LAYER_HEIGHT) - h)});
    rw::call(a::rect_setRect, {ga(&self->mapRect), 0, 0, bits(ldf(ml + LAYER_WIDTH)), bits(ldf(ml + LAYER_HEIGHT))});
    // facing: keep the start info's direction, or face the larger half of the map
    int32_t d = ldi(start + 0xC), nd;
    if (d != 0) nd = d > 0 ? 1 : -1;
    else nd = (ldf(self->mainLayer + LAYER_WIDTH) * 0.5f > ldf(start)) ? -1 : 1;
    rw::save<int32_t>(start + 0xC, nd);
    getCameraMapPosition(f.ptr<bt::Vec2>(0x18), self, rw::host_ptr<bt::Vec2>(start), float(nd));
    if (self->weather) rw::call(a::snowParticle_initCoordinate, {self->weather, f.addr(0x18)});
    updateCamera(self, f.ptr<bt::Vec2>(0x18), 0.0f);
    vc(self->fieldNode, SLOT_SET_POSITION, {self->fieldNode, f.addr(0x10)});
    vc(layer, SLOT_ADD_CHILD, {layer, self->fieldNode});
    float lw = ldf(ml + LAYER_WIDTH);
    self->collisionStride = bt::f2iz((lw + 7.0f) * 0.125f);
    self->jumpStride = bt::f2iz((ldf(ml + LAYER_WIDTH) + 59.0f) / 60.0f);
    self->zoomState = 0;
    self->zoomRequest = 0;
    rw::set<uint32_t>(self, 0xBC, ld(sm + 0x16C));   // materialRange
    self->eventType = 0;
    self->eventTarget = 0;
    return start;
}
RW_FUNCTION(0x7550D4, "StageFieldManager::setStageMapInfo", StageFieldManager::setStageMapInfo);

// Put a map node on the main layer and in the list of its kind.
void StageFieldManager::addMapNodeToField(StageFieldManager *self, uint32_t node) {
    if (!node) return;
    rw::call(a::mapFieldLayer_addFieldMapNode, {self->mainLayer, node, 0});
    rw::call(a::vecMapNode_pushBack, {ga(&self->nodes), node});
    switch (vc(node, SLOT_MAPNODE_KIND, {node}) - 4u) {
    case 0: rw::call(a::vecPrizes_pushBack, {ga(&self->prizes), node}); break;
    case 1: rw::call(a::vecMaterial_pushBack, {ga(&self->materials), node}); break;
    case 2: rw::call(a::vecTreasure_pushBack, {ga(&self->treasures), node}); break;
    case 4: rw::call(a::vecGate_pushBack, {ga(&self->gateMarkers), node}); break;
    case 6: rw::call(a::vecNpc_pushBack, {ga(&self->npcs), node}); break;
    default: break;
    }
}
RW_FUNCTION(0x755508, "StageFieldManager::addMapNodeToField", StageFieldManager::addMapNodeToField);

// Weather 1 = rain (LWF), 2 / 3 = snow particles (style 0 / 1, kept in `weather`).
uint32_t StageFieldManager::getWeatherNode(StageFieldManager *self, uint32_t type, bt::Vec2 *size) {
    uint32_t node = 0;
    if (type == 3 || type == 2) {
        node = rw::call(a::snowParticle_create, {type == 3 ? 1u : 0u});
        self->weather = node;
    } else if (type == 1) {
        node = rw::call(a::lwfNode_create, {a::holeRainLwf, 0});
        float x = size->x * 0.5f;
        float y = size->y * 0.5f;
        vc(node, SLOT_SET_POSITION_XY, {node, bits(x) ^ 0x80000000u, bits(y)});
    }
    return node;
}
RW_FUNCTION(0x7555BC, "StageFieldManager::getWeatherNode", StageFieldManager::getWeatherNode);

float StageFieldManager::getAutoDirectionFromField(StageFieldManager *self, float x) {
    return (ldf(self->mainLayer + LAYER_WIDTH) * 0.5f > x) ? -1.0f : 1.0f;
}
RW_FUNCTION(0x75563C, "StageFieldManager::getAutoDirectionFromField", StageFieldManager::getAutoDirectionFromField);

// Camera position for a player at pos facing `direction`: 100 ahead of him, 80 lower, within the bounds.
void StageFieldManager::getCameraMapPosition(bt::Vec2 *out, StageFieldManager *self, bt::Vec2 *pos, float direction) {
    rw::call(a::vec2_copy, {ga(out), ga(pos)});
    float ahead = direction < 0.0f ? 100.0f : -100.0f;
    out->x = ahead + out->x.get();
    out->y = out->y.get() + -80.0f;
    fitRectToPoint(ga(self), out, &self->cameraBounds);
}
RW_FUNCTION(0x755674, "StageFieldManager::getCameraMapPosition", StageFieldManager::getCameraMapPosition);

// update(Vec2& cameraPos, float dt): move the camera.
void StageFieldManager::updateCamera(StageFieldManager *self, bt::Vec2 *pos, float dt) {
    float x = pos->x, y = pos->y;
    self->cameraPos.y = y;
    self->cameraPos.x = x;
    update_common(self, rw::host_ptr<bt::Vec2>(self->mainLayer + LAYER_WIDTH));
    if (self->weather) rw::call(a::snowParticle_updateCoordinate, {self->weather, ga(pos), bits(dt)});
}
RW_FUNCTION(0x7556E8, "StageFieldManager::update(Vec2&,float)", StageFieldManager::updateCamera);

void StageFieldManager::showMapNode(StageFieldManager *self) {
    uint32_t end = self->npcs.end;
    for (uint32_t p = self->npcs.begin; p != end; p += 4) rw::call(a::npc_showTalkicon, {ld(p), 1});
}
RW_FUNCTION(0x755728, "StageFieldManager::showMapNode", StageFieldManager::showMapNode);

void StageFieldManager::removeMapNodeToField(StageFieldManager *self, uint32_t node) {
    if (!node) return;
    rw::call(a::mapFieldLayer_removeFieldMapNode, {self->mainLayer, node});
    rw::call(a::vecMapNode_erase, {ga(&self->nodes), node, 0});
    switch (vc(node, SLOT_MAPNODE_KIND, {node}) - 4u) {
    case 0: rw::call(a::vecPrizes_erase, {ga(&self->prizes), node, 0}); break;
    case 1: rw::call(a::vecMaterial_erase, {ga(&self->materials), node, 0}); break;
    case 2: rw::call(a::vecTreasure_erase, {ga(&self->treasures), node, 0}); break;
    case 4: rw::call(a::vecGate_erase, {ga(&self->gateMarkers), node, 0}); break;
    case 6: rw::call(a::vecNpc_erase, {ga(&self->npcs), node, 0}); break;
    default: break;
    }
}
RW_FUNCTION(0x7558E4, "StageFieldManager::removeMapNodeToField", StageFieldManager::removeMapNodeToField);

// Move an NPC to the layer with id layerId (0 = the field itself).
void StageFieldManager::addNpcToFieldLayer(StageFieldManager *self, uint32_t npc, int32_t layerId, int32_t z) {
    uint32_t end = self->layers.end;
    for (uint32_t p = self->layers.begin; p != end; p += 4) {
        uint32_t ml = ld(p);
        if (ldi(ml + LAYER_ID) != layerId) continue;
        rw::call(a::mapNode_unlockMapPosZ, {npc});
        if (layerId != 0) rw::call(a::mapFieldLayer_addLayerMapNode, {ml, npc, uint32_t(z)});
        else rw::call(a::mapFieldLayer_addFieldMapNode, {ml, npc, uint32_t(z)});
        return;
    }
}
RW_FUNCTION(0x755CEC, "StageFieldManager::addNpcToFieldLayer", StageFieldManager::addNpcToFieldLayer);

// Ease the field scale toward the requested zoom (1.2 zoomed in, 1.0 normal).
void StageFieldManager::zoomUpdate(StageFieldManager *self) {
    if (self->zoomState == self->zoomRequest) return;
    if (!self->mainLayer) return;
    float scale = fromBits(vc(self->fieldNode, SLOT_GET_SCALE, {self->fieldNode}));
    float to;
    if (scale <= 1.0f && self->zoomRequest == 1) to = 1.2f;
    else if (scale <= 1.2f && self->zoomRequest == 0) to = 1.0f;
    else return;
    uint32_t node = self->fieldNode;
    uint32_t act = rw::call(a::scaleTo_create, {bits(0.5f), bits(to)});
    act = rw::call(a::easeSineInOut_create, {act});
    rw::call(a::node_runAction, {node, act});
    self->zoomState = self->zoomRequest.get();
}
RW_FUNCTION(0x755D40, "StageFieldManager::zoomUpdate", StageFieldManager::zoomUpdate);

// Scroll the layers (parallax ratio of the camera within the map) and depth-sort the shown nodes: each node's
// local z = 128 + the number of shown nodes in front of it (lower map z; ties by index).
void StageFieldManager::update_common(StageFieldManager *self, bt::Vec2 *size) {
    bt::GuestFrame f(8);
    rw::call(a::director_getVisibleSize, {f.addr(0), rw::call(a::director_getInstance, {})});
    float vw = ldf(f.addr(0)), vh = ldf(f.addr(4));
    float v = (self->cameraPos.y - vh * 0.5f) / (size->y - vh);
    if (!(v > 0.0f)) v = 0.0f;
    float ry = (v > 1.0f) ? 1.0f : v;
    v = (self->cameraPos.x - vw * 0.5f) / (size->x - vw);
    if (!(v > 0.0f)) v = 0.0f;
    float rx = (v > 1.0f) ? 1.0f : v;
    uint32_t end = self->layers.end;
    for (uint32_t p = self->layers.begin; p != end; p += 4) rw::call(a::mapFieldLayer_update, {ld(p), bits(rx), bits(ry)});
    int32_t bytes = int32_t(self->nodes.end - self->nodes.begin);
    int32_t n = bytes >> 2;
    if (bytes < 1) return;
    for (int32_t i = 0; i < n; i++) {
        uint32_t node = ld(self->nodes.begin + 4u * uint32_t(i));
        if (vc(node, SLOT_MAPNODE_VISIBLE, {node}) != 1) continue;
        float z = callF(a::mapNode_getMapPosZ, {node});
        uint32_t order = 128;
        for (int32_t j = 0; j < n; j++) {
            uint32_t other = ld(self->nodes.begin + 4u * uint32_t(j));
            if (vc(other, SLOT_MAPNODE_VISIBLE, {other}) != 1) continue;
            float oz = callF(a::mapNode_getMapPosZ, {other});
            if (z > oz) order++;
            else if (i < j && z == oz) order++;
        }
        vc(node, SLOT_NODE_SET_LOCAL_Z, {node, order});
    }
}
RW_FUNCTION(0x755E1C, "StageFieldManager::update_common", StageFieldManager::update_common);

void StageFieldManager::update_raid(StageFieldManager *self) {
    self->cameraPos.y = 0.0f;
    self->cameraPos.x = 0.0f;
    bt::GuestFrame f(8);
    rw::call(a::director_getVisibleSize, {f.addr(0), rw::call(a::director_getInstance, {})});
    update_common(self, f.ptr<bt::Vec2>(0));
}
RW_FUNCTION(0x75603C, "StageFieldManager::update_raid", StageFieldManager::update_raid);

// Per frame: material pick-up, prizes breaking away from their MapPrizes (to the player or to a HUD target),
// finished prize groups leave the field.
void StageFieldManager::update(StageFieldManager *self, float, uint32_t actor) {
    checkMaterial(self, rw::host_ptr<bt::Vec2>(actor + NODE_POS));
    bt::GuestFrame f(12);
    rw::call(a::vecPrizes_copy, {f.addr(), ga(&self->prizes)});
    uint32_t end = f.word(4);
    for (uint32_t p = f.word(0); p != end; p += 4) {
        uint32_t group = ld(p);
        uint32_t b = ld(group + PRIZES_LIST_PLAYER), e = ld(group + PRIZES_LIST_PLAYER + 4);
        if (int32_t(e - b) >= 1)
            for (; b != e; b += 4) {
                uint32_t prize = ld(b);
                if (rw::load<uint8_t>(prize + PRIZE_READY) && rw::call(a::mapPrize_breakAwayArmature, {prize}) == 1)
                    rw::call(a::avatarActor_addPrize, {actor, prize});
            }
        b = ld(group + PRIZES_LIST_ATTRACT), e = ld(group + PRIZES_LIST_ATTRACT + 4);
        if (int32_t(e - b) >= 1)
            for (; b != e; b += 4) {
                uint32_t prize = ld(b);
                if (rw::load<uint8_t>(prize + PRIZE_READY) && rw::call(a::mapPrize_breakAwayArmature, {prize}) == 1)
                    addPrize(self, prize);
            }
        b = ld(group + PRIZES_LIST_MIXED), e = ld(group + PRIZES_LIST_MIXED + 4);
        if (int32_t(e - b) >= 1)
            for (; b != e; b += 4) {
                uint32_t prize = ld(b);
                if (!rw::load<uint8_t>(prize + PRIZE_READY)) continue;
                uint32_t r = rw::call(a::mapPrize_breakAwayArmature, {prize});
                if (r == 1) addPrize(self, prize);
                else delPrize(r, prize);
            }
        if (rw::call(a::mapPrizes_isFinishEruption, {group}) == 1) removeMapNodeToField(self, group);
    }
    rw::call(a::vecPrizes_dtor, {f.addr()});
}
RW_FUNCTION(0x7560B4, "StageFieldManager::update(float,AvatarActor*)", StageFieldManager::update);

// Materials within reach pop up: their prizes go on the field and the material is counted in the StageState.
void StageFieldManager::checkMaterial(StageFieldManager *self, bt::Vec2 *pos) {
    uint32_t end = self->materials.end;
    for (uint32_t p = self->materials.begin; p != end; p += 4) {
        uint32_t m = ld(p);
        if (rw::load<uint8_t>(m + CHEST_OPENED)) continue;
        float d = callF(a::vec2_distance, {m + NODE_POS, ga(pos)});
        if (!(d < self->materialRange)) continue;
        if (rw::call(a::mapMaterial_popup, {m}) != 1) continue;
        bt::GuestFrame f(12);
        rw::call(a::vecPrizes_copy, {f.addr(), m + MATERIAL_PRIZES});
        uint32_t ce = f.word(4);
        const SafeBuffer *sb = rw::host_ptr<SafeBuffer>(m + NODE_SAFEBUFFER);
        for (uint32_t q = f.word(0); q != ce; q += 4) {
            addMapNodeToField(self, ld(q));
            uint32_t st = ld(appState() + 0x20);
            int32_t id = sb->get32(0);
            int32_t count = sb->get32(4);
            rw::call(a::stageState_addMaterial, {st, uint32_t(id), uint32_t(count)});
        }
        rw::call(a::vecPrizes_dtor, {f.addr()});
    }
}
RW_FUNCTION(0x756234, "StageFieldManager::checkMaterial", StageFieldManager::checkMaterial);

// A prize breaking away flies to its HUD target (Lux / items); currencies 0, 1, 4, 8, 9 do not.
void StageFieldManager::addPrize(StageFieldManager *self, uint32_t prize) {
    if (!prize) return;
    bt::GuestFrame f(16);   // MapPrize::Item: type, id, count, flag (byte +0xC)
    f.setWord(0, 1);
    f.setWord(4, 0);
    f.setWord(8, 0);
    rw::store8(f.ptr<uint8_t>(0xC), 0);
    if (rw::call(a::mapPrize_getItem, {prize, f.addr(), 0}) != 1) return;
    uint32_t type = f.word(0);
    if (type <= 11) {
        if ((1u << type) & 0x313u) return;
        if (type == 11) {
            addPrizeAttract(self, prize, &self->luxTarget, 1);
            return;
        }
    }
    if (int32_t(type) > 20) return;
    AttractTarget *t = rw::load<uint8_t>(f.addr(0xC)) ? &self->flaggedItemTarget : &self->itemTarget;
    addPrizeAttract(self, prize, t, 0);
}
RW_FUNCTION(0x75637C, "StageFieldManager::addPrize", StageFieldManager::addPrize);

// A prize that arrived: count it in the StageState (Lux amount / item counters) and remove it.
void StageFieldManager::delPrize(uint32_t, uint32_t prize) {
    if (!prize) return;
    bt::GuestFrame f(16);
    f.setWord(0, 1);
    f.setWord(4, 0);
    f.setWord(8, 0);
    rw::store8(f.ptr<uint8_t>(0xC), 0);
    if (rw::call(a::mapPrize_getItem, {prize, f.addr(), 1}) != 1) return;
    uint32_t type = f.word(0);
    if (type <= 11) {
        if ((1u << type) & 0x313u) return;
        if (type == 11) {
            uint32_t st = ld(appState() + 0x20);
            rw::save<uint32_t>(st + 0x28, ld(st + 0x28) + f.word(8));
            vc(prize, SLOT_REMOVE_FROM_PARENT, {prize});
            return;
        }
    }
    if (int32_t(type) > 20) return;
    uint32_t app = appState();
    uint32_t st = ld(app + 0x20);
    uint32_t ctrl = ld(app + 0x24);   // shared_ptr<StageState> copy
    if (ctrl) rw::call(a::add_shared, {ctrl});
    if (rw::load<uint8_t>(f.addr(0xC))) rw::save<uint32_t>(st + 0x34, ld(st + 0x34) + 1);
    else rw::save<uint32_t>(st + 0x30, ld(st + 0x30) + 1);
    vc(prize, SLOT_REMOVE_FROM_PARENT, {prize});
    bt::releaseShared(ctrl);
}
RW_FUNCTION(0x756424, "StageFieldManager::delPrize", StageFieldManager::delPrize);

// ============================================================================================ coordinates
void StageFieldManager::getLocalPosToMapPos(bt::Vec2 *out, StageFieldManager *self, bt::Vec2 *pos) {
    float x = self->cameraPos.x + pos->x;
    float y = self->cameraPos.y - pos->y;
    rw::call(a::vec2_ctor_ff, {ga(out), bits(x), bits(y)});
}
RW_FUNCTION(0x75651C, "StageFieldManager::getLocalPosToMapPos", StageFieldManager::getLocalPosToMapPos);

void StageFieldManager::getScreenPosToMapPos(bt::Vec2 *out, StageFieldManager *self, bt::Vec2 *pos) {
    bt::GuestFrame f(8);
    rw::call(a::vec2_copy, {f.addr(), ga(pos)});
    rw::call(a::vec2_subtract, {f.addr(), ga(&self->screenCenter)});
    rw::call(a::vec2_copy, {ga(out), ga(&self->cameraPos)});
    rw::call(a::vec2_add, {ga(out), f.addr()});
}
RW_FUNCTION(0x756558, "StageFieldManager::getScreenPosToMapPos", StageFieldManager::getScreenPosToMapPos);

void StageFieldManager::getMapPosToScreenPos(bt::Vec2 *out, StageFieldManager *self, bt::Vec2 *pos) {
    rw::call(a::vec2_copy, {ga(out), ga(pos)});
    rw::call(a::vec2_subtract, {ga(out), ga(&self->cameraPos)});
    out->y = -0.0f - out->y.get();
}
RW_FUNCTION(0x7565F0, "StageFieldManager::getMapPosToScreenPos", StageFieldManager::getMapPosToScreenPos);

// Clamp p into rect; true if it moved.
uint32_t StageFieldManager::fitRectToPoint(uint32_t, bt::Vec2 *p, SfmRect *rect) {
    uint32_t r = ga(rect);
    uint32_t moved = 0;
    float v = p->x;
    if (v < rectF(a::rect_getMinX, r)) {
        uint32_t m = rw::call(a::rect_getMinX, {r});
        rw::store32(&p->x, m);
        v = fromBits(m);
        moved = 1;
    } else {
        v = p->x;
    }
    if (v > rectF(a::rect_getMaxX, r)) {
        uint32_t m = rw::call(a::rect_getMaxX, {r});
        rw::store32(&p->x, m);
        moved = 1;
    }
    v = p->y;
    if (v < rectF(a::rect_getMinY, r)) {
        uint32_t m = rw::call(a::rect_getMinY, {r});
        rw::store32(&p->y, m);
        v = fromBits(m);
        moved = 1;
    } else {
        v = p->y;
    }
    if (v > rectF(a::rect_getMaxY, r)) {
        rw::store32(&p->y, rw::call(a::rect_getMaxY, {r}));
        return 1;
    }
    return moved;
}
RW_FUNCTION(0x756632, "StageFieldManager::fitRectToPoint", StageFieldManager::fitRectToPoint);

// ============================================================================================ collision
// From the (rounded) position walk up to 30 steps in 6 directions while the way is free; directions 2..5 stop
// early (safe) when the side probe is free. Safe unless the walls left and right of the position are symmetric.
uint32_t StageFieldManager::isCollisionRectSafe(StageFieldManager *self, bt::Vec2 *pos) {
    int32_t x = bt::f2iz(roundF(pos->x));
    int32_t y = bt::f2iz(roundF(pos->y));
    int32_t px[6], py[6];
    for (int k = 0; k < 6; k++) px[k] = x, py[k] = y;
    if (isCollisionSafe(self, x, y) != 1) return 0;
    int32_t cx = x, cy = y;
    for (uint32_t d = 0; d < 6; d++) {
        int32_t dx = ldi(a::collisionDirections + 8 * d), dy = ldi(a::collisionDirections + 8 * d + 4);
        uint32_t side = a::collisionSideChecks + 8 * d;
        for (int32_t steps = 0;;) {
            int32_t nx = int32_t(uint32_t(dx) + uint32_t(cx));
            int32_t ny = int32_t(uint32_t(dy) + uint32_t(cy));
            if (isCollisionSafe(self, nx, ny) != 1) break;
            if (d - 2u <= 3u) {
                int32_t sx = int32_t(uint32_t(cx) + ld(side)), sy = int32_t(uint32_t(cy) + ld(side + 4));
                if (isCollisionSafe(self, sx, sy) != 0) return 1;
            }
            steps++;
            cx = nx;
            cy = ny;
            if (steps >= 30) return 1;
        }
        px[d] = cx;
        py[d] = cy;
    }
    if (int32_t(uint32_t(px[2]) - uint32_t(px[5])) != int32_t(uint32_t(px[3]) - uint32_t(px[4]))) return 1;
    return int32_t(uint32_t(py[4]) - uint32_t(py[5])) != int32_t(uint32_t(py[3]) - uint32_t(py[2])) ? 1 : 0;
}
RW_FUNCTION(0x756734, "StageFieldManager::isCollisionRectSafe", StageFieldManager::isCollisionRectSafe);

// A pixel of the collision bitmap (Data bytes +0x10: header) is free (bit clear).
uint32_t StageFieldManager::isCollisionSafe(StageFieldManager *self, int32_t x, int32_t y) {
    if ((x | y) < 0) return 0;
    uint32_t ml = self->mainLayer;
    float fx = float(x), w = ldf(ml + LAYER_WIDTH);
    if (fx >= w || std::isnan(fx) || std::isnan(w)) return 0;
    float fy = float(y), h = ldf(ml + LAYER_HEIGHT);
    if (fy >= h || std::isnan(fy) || std::isnan(h)) return 0;
    int32_t idx = int32_t(uint32_t(self->collisionStride) * uint32_t(y) + uint32_t(x >> 3));
    if (idx >= int32_t(rw::call(a::data_getSize, {self->collisionData}))) return 0;
    uint32_t mask = 0x80u >> (x & 7);
    uint8_t byte = rw::load<uint8_t>(uint32_t(idx) + rw::call(a::data_getBytes, {self->collisionData}) + 0x10);
    return (byte & mask) ? 0 : 1;
}
RW_FUNCTION(0x7568A8, "StageFieldManager::isCollisionSafe(int,int)", StageFieldManager::isCollisionSafe);

uint32_t StageFieldManager::isCollisionSafeVec(StageFieldManager *self, bt::Vec2 *pos) {
    int32_t x = bt::f2iz(roundF(pos->x));
    int32_t y = bt::f2iz(roundF(pos->y));
    return isCollisionSafe(self, x, y);
}
RW_FUNCTION(0x756980, "StageFieldManager::isCollisionSafe(Vec2)", StageFieldManager::isCollisionSafeVec);

uint32_t StageFieldManager::isCollisionSafeF(StageFieldManager *self, float x, float y) {
    int32_t ix = bt::f2iz(roundF(x));
    int32_t iy = bt::f2iz(roundF(y));
    return isCollisionSafe(self, ix, iy);
}
RW_FUNCTION(0x7569B4, "StageFieldManager::isCollisionSafe(float,float)", StageFieldManager::isCollisionSafeF);

uint32_t StageFieldManager::isCollisionDetectionVec(StageFieldManager *self, bt::Vec2 *from, bt::Vec2 *to) {
    int32_t y1 = bt::f2iz(roundF(to->y));
    int32_t x0 = bt::f2iz(roundF(from->x));
    int32_t y0 = bt::f2iz(roundF(from->y));
    int32_t x1 = bt::f2iz(roundF(to->x));
    return isCollisionDetection(self, x0, y0, x1, y1);
}
RW_FUNCTION(0x7569EC, "StageFieldManager::isCollisionDetection(Vec2,Vec2)", StageFieldManager::isCollisionDetectionVec);

// Bresenham walk from (x0, y0) to (x1, y1): true when a blocked pixel is hit (or at once when both are equal).
uint32_t StageFieldManager::isCollisionDetection(StageFieldManager *self, int32_t x0, int32_t y0, int32_t x1,
                                                 int32_t y1) {
    int32_t sy = y1 > y0 ? 1 : -1;
    int32_t sx = x1 > x0 ? 1 : -1;
    int32_t ddy = int32_t(uint32_t(y1) - uint32_t(y0)), ddx = int32_t(uint32_t(x1) - uint32_t(x0));
    int32_t dy = int32_t((uint32_t(ddy) + uint32_t(ddy >> 31)) ^ uint32_t(ddy >> 31));
    int32_t dx = int32_t((uint32_t(ddx) + uint32_t(ddx >> 31)) ^ uint32_t(ddx >> 31));
    int32_t err = int32_t(uint32_t(dx) - uint32_t(dy));
    int32_t negDy = int32_t(0u - uint32_t(dy));
    uint32_t hit = 1;
    for (;;) {
        if (y0 == y1 && x0 == x1) break;
        int32_t e2 = int32_t(uint32_t(err) << 1);
        if (e2 > negDy) {
            x0 = int32_t(uint32_t(x0) + uint32_t(sx));
            err = int32_t(uint32_t(err) - uint32_t(dy));
        }
        if (e2 < dx) {
            y0 = int32_t(uint32_t(y0) + uint32_t(sy));
            err = int32_t(uint32_t(err) + uint32_t(dx));
        }
        uint32_t safe = isCollisionSafe(self, x0, y0);
        hit = safe ^ 1;
        if (safe == 0) break;
    }
    return hit & 1;
}
RW_FUNCTION(0x756A48, "StageFieldManager::isCollisionDetection(int,int,int,int)", StageFieldManager::isCollisionDetection);

// Standing on a jump cell (60x60 grid value 2) and tapping toward the nearest jump of that cell: jump along a
// Bezier to its landing point. True when the jump starts.
uint32_t StageFieldManager::hasJumpData(StageFieldManager *self, uint32_t actor, bt::Vec2 *target) {
    // frame offsets as the original's: +0x1C Bezier config (end, cp1, cp2), +0x34 tap dir, +0x3C jump dir,
    // +0x44 landing, +0x4C take-off, +0x54 cell key, +0x58 actor position
    bt::GuestFrame f(0x60);
    rw::call(a::vec2_copy, {f.addr(0x58), actor + NODE_POS});
    int32_t cx = bt::f2iz(roundF(ldf(f.addr(0x58)))) / 60;
    int32_t cy = bt::f2iz(roundF(ldf(f.addr(0x5C)))) / 60;
    int32_t cell = int32_t(uint32_t(self->jumpStride) * uint32_t(cy) + uint32_t(cx));
    f.setWord(0x54, uint32_t(cell));
    if (rw::load<uint8_t>(uint32_t(cell) + rw::call(a::data_getBytes, {self->jumpData}) + 0x10) != 2) return 0;
    uint32_t map = self->jumpInfoMap;
    if (!rw::call(a::jumpMap_find, {map, f.addr(0x54)})) return 0;
    uint32_t vec = ld(rw::call(a::jumpMap_at, {map, f.addr(0x54)}));
    uint32_t p = ld(vec), end = ld(vec + 4);
    float best = fromBits(0x7F7FFFFFu);
    uint32_t chosen = 0;
    if (p == end) return 0;
    for (; p != end; p += 4) {
        uint32_t info = ld(p);
        float d = callF(a::vec2_distanceSquared, {ga(target), info});
        if (!(d > best || std::isnan(d) || std::isnan(best))) {
            chosen = info;
            best = d;
        }
    }
    if (!chosen) return 0;
    rw::call(a::vec2_copy, {f.addr(0x4C), chosen});
    rw::call(a::vec2_subtract, {f.addr(0x4C), f.addr(0x58)});
    rw::call(a::vec2_copy, {f.addr(0x44), chosen + 8});
    rw::call(a::vec2_subtract, {f.addr(0x44), f.addr(0x58)});
    rw::call(a::vec2_copy, {f.addr(0x3C), f.addr(0x4C)});
    rw::call(a::vec2_copy, {f.addr(0x34), ga(target)});
    rw::call(a::vec2_subtract, {f.addr(0x34), f.addr(0x58)});
    if (rw::call(a::vec2_isZero, {f.addr(0x34)}) != 0) return 0;
    rw::call(a::vec2_normalize, {f.addr(0x3C)});
    rw::call(a::vec2_normalize, {f.addr(0x34)});
    float dot = callF(a::vec2_dot, {f.addr(0x3C), f.addr(0x34)});
    if (dot <= 0.0f || std::isnan(dot)) return 0;
    // map y grows downward, the node's y upward
    rw::save<float>(f.addr(0x50), -0.0f - ldf(f.addr(0x50)));
    rw::save<float>(f.addr(0x48), -0.0f - ldf(f.addr(0x48)));
    rw::call(a::vec2_copy, {f.addr(0x1C), f.addr(0x4C)});
    rw::call(a::vec2_copy, {f.addr(0x24), f.addr(0x44)});
    rw::call(a::vec2_copy, {f.addr(0x2C), f.addr(0x44)});
    float duration = float(ldi(chosen + 0x10) >> 1) / 30.0f;
    uint32_t act = rw::call(a::bezierBy_create, {bits(duration), f.addr(0x1C)});
    rw::call(a::mapNode_move, {actor, act});
    rw::call(a::avatarActor_lookAt, {actor, chosen});
    return 1;
}
RW_FUNCTION(0x756AD4, "StageFieldManager::hasJumpData", StageFieldManager::hasJumpData);

// ============================================================================================ events
void StageFieldManager::onOpenTreasure(StageFieldManager *self, uint32_t treasure) {
    uint32_t end = ld(treasure + 0x2B0);
    for (uint32_t p = ld(treasure + MATERIAL_PRIZES); p != end; p += 4) addMapNodeToField(self, ld(p));
}
RW_FUNCTION(0x756E2C, "StageFieldManager::onOpenTreasure", StageFieldManager::onOpenTreasure);

// A tap near an unopened treasure (100) or a talkable NPC (its tap range) becomes the pending event.
uint32_t StageFieldManager::checkEventAreaTap(StageFieldManager *self, bt::Vec2 *pos) {
    uint32_t end = self->treasures.end;
    for (uint32_t p = self->treasures.begin; p != end; p += 4) {
        uint32_t t = ld(p);
        if (ld(t + CHEST_OPENED) != 0) continue;
        float d = callF(a::vec2_distance, {t + NODE_POS, ga(pos)});
        if (!(d < 100.0f)) continue;
        self->eventType = 1;
        uint32_t x = ld(t + NODE_POS), y = ld(t + NODE_POS + 4);
        rw::set<uint32_t>(self, 0xC8, y);
        rw::set<uint32_t>(self, 0xC4, x);
        self->eventRange = 100.0f;
        self->eventTarget = t;
        return 1;
    }
    end = self->npcs.end;
    for (uint32_t p = self->npcs.begin; p != end; p += 4) {
        uint32_t npc = ld(p);
        int32_t state = ldi(npc + 0x2B4);
        if (state == 2 || state == 4) continue;
        bt::GuestFrame f(8);
        rw::call(a::npc_getTapCenterMapPosition, {f.addr(), npc});
        float d = callF(a::vec2_distance, {f.addr(), ga(pos)});
        float range = callF(a::npc_getTapRange, {npc});
        if (!(d < range)) continue;
        self->eventType = 2;
        uint32_t x = ld(npc + NODE_POS), y = ld(npc + NODE_POS + 4);
        rw::set<uint32_t>(self, 0xC8, y);
        rw::set<uint32_t>(self, 0xC4, x);
        self->eventRange = 120.0f;
        self->eventTarget = npc;
        return 1;
    }
    self->eventType = 0;
    self->eventTarget = 0;
    return 0;
}
RW_FUNCTION(0x756E54, "StageFieldManager::checkEventAreaTap", StageFieldManager::checkEventAreaTap);

// The player reached the pending event: talk to the NPC (when the stage is in state 2) or open the treasure (when
// nothing blocks the way). True when it happened; the pending event is cleared either way once in range.
uint32_t StageFieldManager::checkEventOccurs(StageFieldManager *self, uint32_t sm, uint32_t actor) {
    if (ldi(ld(actor + ACTOR_AVATAR) + 0x524) == 16) return 0;
    if (self->eventType == 0) return 0;
    float d = callF(a::vec2_distance, {ga(&self->eventPos), actor + NODE_POS});
    float range = self->eventRange;
    if (d > range || std::isnan(d) || std::isnan(range)) return 0;
    uint32_t happened = 0;
    if (self->eventType == 2) {
        if (ldi(sm + 0x18) == 2) {
            onEventOccursNpc(self, self->eventTarget, actor, sm);
            happened = 1;
        }
    } else if (self->eventType == 1) {
        if (isCollisionDetectionVec(self, rw::host_ptr<bt::Vec2>(actor + NODE_POS), &self->eventPos) == 0) {
            onEventOccursTreasure(self, self->eventTarget, actor);
            happened = 1;
        }
    }
    self->eventType = 0;
    self->eventTarget = 0;
    return happened;
}
RW_FUNCTION(0x756FD4, "StageFieldManager::checkEventOccurs", StageFieldManager::checkEventOccurs);

void StageFieldManager::onEventOccursTreasure(StageFieldManager *self, uint32_t treasure, uint32_t actor) {
    // std::function<void(MapTreasure*)> holding the lambda [this]: buffer +0 (vtable, this), __f_ at +16
    bt::GuestFrame f(24);
    f.setWord(0, a::openTreasureLambdaVtable);
    f.setWord(4, ga(self));
    f.setWord(16, f.addr(0));
    uint32_t opened = rw::call(a::mapTreasure_open, {treasure, f.addr(0)});
    uint32_t fn = f.word(16);
    if (fn == f.addr(0)) rw::call(ld(ld(fn) + 0x10), {fn});        // destroy()
    else if (fn) rw::call(ld(ld(fn) + 0x14), {fn});                // destroy_deallocate()
    if (opened != 1) return;
    uint32_t st = ld(appState() + 0x20);
    const SafeBuffer *sb = rw::host_ptr<SafeBuffer>(treasure + NODE_SAFEBUFFER);
    int32_t count = sb->get32(4);
    int32_t id = sb->get32(0);
    rw::call(a::stageState_addTreasure, {st, uint32_t(count), uint32_t(id)});
    rw::call(a::avatarActor_lookAt, {actor, treasure + NODE_POS});
    rw::call(a::avatar_changeMotion, {ld(actor + ACTOR_AVATAR), 27});
}
RW_FUNCTION(0x7570A8, "StageFieldManager::onEventOccursTreasure", StageFieldManager::onEventOccursTreasure);

void StageFieldManager::onEventOccursNpc(StageFieldManager *self, uint32_t npc, uint32_t actor, uint32_t sm) {
    rw::call(a::npc_lookAt, {npc, actor + NODE_POS});
    rw::call(a::avatarActor_lookAt, {actor, npc + NODE_POS});
    rw::call(a::avatar_changeMotion, {ld(actor + ACTOR_AVATAR), 27});
    rw::save<uint32_t>(sm + 0x18, 5);   // StageManager state: talking
    rw::save<uint32_t>(sm + 0x1C, 0);
    self->talkingNpc = npc;
    rw::call(a::npc_talk, {npc, sm});
    uint32_t st = ld(appState() + 0x20);
    int32_t id = rw::host_ptr<SafeBuffer>(npc + NODE_SAFEBUFFER)->get32(0);
    rw::call(a::stageState_addActionAlreadyNPC, {st, uint32_t(id)});
}
RW_FUNCTION(0x7571C4, "StageFieldManager::onEventOccursNpc", StageFieldManager::onEventOccursNpc);

uint32_t StageFieldManager::getCurrentEventMapTreasure(StageFieldManager *self) {
    return self->eventType == 1 ? self->eventTarget.get() : 0;
}
RW_FUNCTION(0x757274, "StageFieldManager::getCurrentEventMapTreasure", StageFieldManager::getCurrentEventMapTreasure);

uint32_t StageFieldManager::isProcessedPrizes(StageFieldManager *self) {
    if (self->eventType == 1 && self->eventTarget != 0) return 1;
    uint32_t end = self->treasures.end;
    for (uint32_t p = self->treasures.begin; p != end; p += 4)
        if (ldi(ld(p) + CHEST_OPENED) == 1) return 1;
    return int32_t(self->prizes.end - self->prizes.begin) > 0 ? 1 : 0;
}
RW_FUNCTION(0x757288, "StageFieldManager::isProcessedPrizes", StageFieldManager::isProcessedPrizes);

// ============================================================================================ attract targets
void StageFieldManager::setLuxTarget(StageFieldManager *self, uint32_t target, uint32_t parent) {
    if (!target || !parent) return;
    uint32_t node = setTargetNode(self, &self->luxTarget, target);
    vc(parent, SLOT_ADD_CHILD_Z, {parent, node, 0x7FFFFFFF});
}
RW_FUNCTION(0x7572D0, "StageFieldManager::setLuxTarget", StageFieldManager::setLuxTarget);

// The target's offset = (HUD node's world position - screen centre) / 1.2, y flipped; a new node holds it.
uint32_t StageFieldManager::setTargetNode(StageFieldManager *self, AttractTarget *target, uint32_t hudNode) {
    bt::GuestFrame f(16);   // +0 offset, +8 world position
    rw::call(a::node_convertToWorldSpace, {f.addr(8), hudNode, a::vec2_ZERO});
    rw::call(a::vec2_copy, {f.addr(0), f.addr(8)});
    rw::call(a::vec2_subtract, {f.addr(0), ga(&self->screenCenter)});
    rw::call(a::vec2_scale, {f.addr(0), 0x3F555555u});
    uint32_t x = f.word(0), y = f.word(4);
    rw::store32(&target->offset.x, x);
    rw::store32(&target->offset.y, y);
    rw::store32(&target->offset.y, y ^ 0x80000000u);
    uint32_t node = rw::call(a::node_create, {});
    target->node = node;
    vc(node, SLOT_SET_POSITION, {node, f.addr(0)});
    return target->node;
}
RW_FUNCTION(0x757304, "StageFieldManager::setTargetNode", StageFieldManager::setTargetNode);

void StageFieldManager::setTreasureTarget(StageFieldManager *self, uint32_t itemNode, uint32_t flaggedNode,
                                          uint32_t parent) {
    if (!parent) return;
    if (itemNode) {
        uint32_t node = setTargetNode(self, &self->itemTarget, itemNode);
        vc(parent, SLOT_ADD_CHILD_Z, {parent, node, 0x7FFFFFFF});
    }
    if (flaggedNode) {
        uint32_t node = setTargetNode(self, &self->flaggedItemTarget, flaggedNode);
        vc(parent, SLOT_ADD_CHILD_Z, {parent, node, 0x7FFFFFFF});
    }
}
RW_FUNCTION(0x7573D8, "StageFieldManager::setTreasureTarget", StageFieldManager::setTreasureTarget);

// Count every prize still flying to the target as arrived.
void StageFieldManager::clearTargetNode(uint32_t, AttractTarget *target) {
    uint32_t node = target->node;
    if (!node) return;
    uint32_t children = vc(node, SLOT_GET_CHILDREN, {node});
    bt::GuestFrame f(12);
    rw::call(a::vecNode_copy, {f.addr(), children});
    uint32_t end = f.word(4);
    for (uint32_t p = f.word(0); p != end; p += 4) delPrize(0, ld(p));
    rw::call(a::vecNode_dtor, {f.addr()});
}
RW_FUNCTION(0x757444, "StageFieldManager::clearTargetNode", StageFieldManager::clearTargetNode);

// Send a prize flying to the target (it becomes the target node's child); without a target it arrives at once.
void StageFieldManager::addPrizeAttract(StageFieldManager *self, uint32_t prize, AttractTarget *target, uint32_t flag) {
    if (!target->node) {
        delPrize(0, prize);
        return;
    }
    // frame: +0x00 target map position, +0x08 offset to the prize, +0x10 std::function (buffer, __f_ at +0x20)
    bt::GuestFrame f(0x28);
    rw::call(a::vec2_copy, {f.addr(0x00), ga(&self->cameraPos)});
    rw::call(a::vec2_add, {f.addr(0x00), ga(&target->offset)});
    rw::call(a::vec2_copy, {f.addr(0x08), prize + NODE_POS});
    rw::call(a::vec2_subtract, {f.addr(0x08), f.addr(0x00)});
    rw::save<float>(f.addr(0x0C), -0.0f - ldf(f.addr(0x0C)));
    f.setWord(0x20, f.addr(0x10));
    f.setWord(0x10, a::attractLambdaVtable);
    f.setWord(0x14, ga(self));
    rw::call(a::mapPrize_setAbsorptionAttract, {prize, f.addr(0x08), f.addr(0x10), flag});
    uint32_t fn = f.word(0x20);
    if (fn == f.addr(0x10)) rw::call(ld(ld(fn) + 0x10), {fn});     // destroy()
    else if (fn) rw::call(ld(ld(fn) + 0x14), {fn});                // destroy_deallocate()
    uint32_t node = target->node;
    vc(node, SLOT_ADD_CHILD, {node, prize});
}
RW_FUNCTION(0x7574C4, "StageFieldManager::addPrizeAttract", StageFieldManager::addPrizeAttract);

// Quest end: close treasures, every prize still on the field counts as collected.
void StageFieldManager::terminateCollectPrize(StageFieldManager *self, uint32_t actor) {
    uint32_t end = self->treasures.end;
    for (uint32_t p = self->treasures.begin; p != end; p += 4) rw::call(a::mapTreasure_terminate, {ld(p)});
    end = self->prizes.end;
    for (uint32_t p = self->prizes.begin; p != end; p += 4) {
        uint32_t group = ld(p);
        uint32_t b = ld(group + PRIZES_LIST_PLAYER), e = ld(group + PRIZES_LIST_PLAYER + 4);
        if (int32_t(e - b) >= 1)
            for (; b != e; b += 4)
                if (!rw::load<uint8_t>(ld(b) + PRIZE_READY)) rw::call(a::avatarActor_delPrize, {actor, ld(b), 0});
        b = ld(group + PRIZES_LIST_ATTRACT), e = ld(group + PRIZES_LIST_ATTRACT + 4);
        if (int32_t(e - b) >= 1)
            for (; b != e; b += 4)
                if (!rw::load<uint8_t>(ld(b) + PRIZE_READY)) delPrize(0, ld(b));
        b = ld(group + PRIZES_LIST_MIXED), e = ld(group + PRIZES_LIST_MIXED + 4);
        if (int32_t(e - b) >= 1)
            for (; b != e; b += 4)
                if (!rw::load<uint8_t>(ld(b) + PRIZE_READY)) delPrize(0, ld(b));
    }
    clearTargetNode(0, &self->luxTarget);
    clearTargetNode(0, &self->flaggedItemTarget);
    clearTargetNode(0, &self->itemTarget);
    rw::call(a::avatarActor_clearPrize, {actor});
}
RW_FUNCTION(0x7575F0, "StageFieldManager::terminateCollectPrize", StageFieldManager::terminateCollectPrize);

// Battle on: gate markers and NPC talk icons hide; battle off: they show. The pending event is dropped.
void StageFieldManager::changeMode(StageFieldManager *self, uint32_t battle) {
    uint32_t show = battle ^ 1u;
    uint32_t end = self->gateMarkers.end;
    for (uint32_t p = self->gateMarkers.begin; p != end; p += 4) vc(ld(p), SLOT_GATE_SET_ENABLE, {ld(p), show});
    end = self->npcs.end;
    for (uint32_t p = self->npcs.begin; p != end; p += 4) rw::call(a::npc_showTalkicon, {ld(p), show});
    self->eventType = 0;
    self->eventTarget = 0;
}
RW_FUNCTION(0x7576D2, "StageFieldManager::changeMode", StageFieldManager::changeMode);

// ============================================================================================ enemy state boards
// Keep each enemy's state board (142x40 above it) on screen and off the HUD: push the board's screen rect inside
// the screen, step it vertically out of the first HUD group it overlaps (bisecting), then ease the board node
// toward the corrected position (factor 0.8^(dt / animation interval)).
void StageFieldManager::reapStateBoard(StageFieldManager *self, float dt, bt::Vector *enemies) {
    // frame offsets as the original's sp offsets: +0x58 R, +0x60 Q, +0x68 S, +0x70 P (Vec2), +0x7C D, +0x84 tmp,
    // +0x8C board screen rect (moved), +0x9C board screen rect, +0xAC board map rect, +0xBC to, +0xC4 from,
    // +0xCC screen rect
    bt::GuestFrame f(0xE0);
    float sx = callF(a::appGetFrameWidth, {}) * -0.5f;
    float sy = callF(a::appGetFrameHeight, {}) * -0.5f;
    float sw = callF(a::appGetFrameWidth, {});
    float sh = callF(a::appGetFrameHeight, {});
    rw::call(a::rect_ctor4, {f.addr(0xCC), bits(sx), bits(sy), bits(sw), bits(sh)});
    float scale = fromBits(vc(self->fieldNode, SLOT_GET_SCALE, {self->fieldNode}));
    float inv = 1.0f / scale;
    uint32_t director = rw::call(a::director_getInstance, {});
    float t = float(double(dt) / rw::load<double>(director + 0xA8));
    float k = callF(a::powf_, {bits(0.8f), bits(t)});
    float km = 1.0f - k;
    uint32_t begin = enemies->begin;
    int32_t bytes = int32_t(enemies->end - begin);
    if (bytes < 1) return;
    uint32_t cam = ga(&self->cameraPos);
    for (int32_t i = bytes >> 2;;) {
        int32_t j = int32_t(uint32_t(i) - 1u);
        uint32_t e = ld(begin + 4u * uint32_t(j));
        if (ldi(e + 0x2A0) == 3) {
            rw::call(a::vec2_copy, {f.addr(0xC4), a::vec2_ZERO});
            rw::call(a::vec2_copy, {f.addr(0xBC), a::vec2_ZERO});
            float rx = (ldf(e + NODE_POS) + ldf(e + 0x3C4)) + -71.0f;
            float ry = (ldf(e + NODE_POS + 4) - ldf(e + 0x3C8)) + -40.0f;
            uint32_t boardPos = e + 0x3C4;   // board offset from the enemy
            uint32_t board = ld(e + 0x3C0);  // state board node
            rw::call(a::rect_ctor4, {f.addr(0xAC), bits(rx), bits(ry), bits(142.0f), bits(40.0f)});
            float x0 = scale * (rectF(a::rect_getMinX, f.addr(0xAC)) - ldf(cam));
            float y0 = scale * (rectF(a::rect_getMinY, f.addr(0xAC)) - ldf(cam + 4));
            float mx = rectF(a::rect_getMaxX, f.addr(0xAC));
            float hh = scale * (rectF(a::rect_getMaxY, f.addr(0xAC)) - ldf(cam + 4)) - y0;
            float ww = scale * (mx - ldf(cam)) - x0;
            rw::call(a::rect_ctor4, {f.addr(0x9C), bits(x0), bits(y0), bits(ww), bits(hh)});
            rw::call(a::rect_copy, {f.addr(0x8C), f.addr(0x9C)});
            uint32_t r = f.addr(0x8C), scr = f.addr(0xCC);
            if (rectF(a::rect_getMinX, r) < rectF(a::rect_getMinX, scr)) {
                f.setWord(0xC4, rw::call(a::rect_getMinX, {r}));
                f.setWord(0xBC, rw::call(a::rect_getMinX, {scr}));
            }
            if (rectF(a::rect_getMaxX, r) > rectF(a::rect_getMaxX, scr)) {
                f.setWord(0xC4, rw::call(a::rect_getMaxX, {r}));
                f.setWord(0xBC, rw::call(a::rect_getMaxX, {scr}));
            }
            if (rectF(a::rect_getMinY, r) < rectF(a::rect_getMinY, scr)) {
                f.setWord(0xC8, rw::call(a::rect_getMinY, {r}));
                f.setWord(0xC0, rw::call(a::rect_getMinY, {scr}));
            }
            if (rectF(a::rect_getMaxY, r) > rectF(a::rect_getMaxY, scr)) {
                f.setWord(0xC8, rw::call(a::rect_getMaxY, {r}));
                f.setWord(0xC0, rw::call(a::rect_getMaxY, {scr}));
            }
            rw::call(a::vec2_copy, {f.addr(0x84), f.addr(0xBC)});
            rw::call(a::vec2_subtract, {f.addr(0x84), f.addr(0xC4)});
            rw::call(a::vec2_add, {r, f.addr(0x84)});
            uint32_t group = 0;
            bool found = false;
            for (uint32_t off = 0; off != 0x40; off += 0x10) {
                group = a::stateBoardGroups + off;
                if (rw::call(a::hitShapeGroupInfo_isHitTest, {group, r, scr}) == 1) {
                    found = true;
                    break;
                }
            }
            if (found) {
                // move by step until the hit state flips, then reverse with half the step
                int32_t step = bt::f2iz(ldf(group + 8) * 128.0f);
                uint32_t flag = 0;
                for (;;) {
                    uint32_t want = flag & 1;
                    float fs = float(step);
                    do {
                        rw::save<float>(f.addr(0x90), fs + ldf(f.addr(0x90)));
                    } while ((rw::call(a::hitShapeGroupInfo_isHitTest, {group, r, scr}) ^ 1u) == want);
                    flag ^= 1;
                    int32_t old = step;
                    step = int32_t(0u - uint32_t(old / 2));
                    if (!(uint32_t(old) + 1u > 2u)) break;
                }
            }
            rw::call(a::vec2_copy, {f.addr(0x7C), r});
            rw::call(a::vec2_subtract, {f.addr(0x7C), f.addr(0x9C)});
            float dx = ldf(f.addr(0x7C)), dy = ldf(f.addr(0x80));
            if (!(dx == 0.0f && dy == 0.0f)) {
                rw::call(a::vec2_copy, {f.addr(0x70), boardPos});
                rw::save<float>(f.addr(0x70), ldf(f.addr(0x70)) + inv * dx);
                rw::save<float>(f.addr(0x74), ldf(f.addr(0x74)) - inv * dy);
                rw::call(a::vec2_copy, {f.addr(0x60), f.addr(0x70)});
                rw::call(a::vec2_scale, {f.addr(0x60), bits(km)});
                uint32_t cur = vc(board, SLOT_GET_POSITION, {board});
                rw::call(a::vec2_copy, {f.addr(0x58), cur});
                rw::call(a::vec2_scale, {f.addr(0x58), bits(k)});
                rw::call(a::vec2_copy, {f.addr(0x68), f.addr(0x60)});
                rw::call(a::vec2_add, {f.addr(0x68), f.addr(0x58)});
                f.setWord(0x70, f.word(0x68));
                f.setWord(0x74, f.word(0x6C));
                vc(board, SLOT_SET_POSITION, {board, f.addr(0x70)});
            }
        }
        if (i < 2) break;
        i = j;
        begin = enemies->begin;
    }
}
RW_FUNCTION(0x757720, "StageFieldManager::reapStateBoard", StageFieldManager::reapStateBoard);

// Does the enemy's touch rect overlap a HUD touch area (screen rect corners against the HUD shapes)?
uint32_t StageFieldManager::isHitTestTouchRectAndTouchUI(StageFieldManager *self, uint32_t enemy) {
    // frame: +0x10 / +0x18 corner points, +0x20 enemy touch rect, +0x30 screen rect (map coordinates)
    bt::GuestFrame f(0x40);
    float cx = self->cameraPos.x;
    float x = cx - callF(a::appGetFrameWidth, {}) * 0.5f;
    float cy = self->cameraPos.y;
    float y = cy - callF(a::appGetFrameHeight, {}) * 0.5f;
    float w = callF(a::appGetFrameWidth, {});
    float h = callF(a::appGetFrameHeight, {});
    rw::call(a::rect_ctor4, {f.addr(0x30), bits(x), bits(y), bits(w), bits(h)});
    rw::call(a::enemyActor_getMapPosotionTouchRect, {f.addr(0x20), enemy});
    uint32_t minX = rw::call(a::rect_getMinX, {f.addr(0x30)});
    uint32_t maxY = rw::call(a::rect_getMaxY, {f.addr(0x30)});
    rw::call(a::vec2_ctor_ff, {f.addr(0x18), minX, maxY});
    uint32_t shape = a::touchUiShapes;
    for (uint32_t n = 0;;) {
        uint32_t hit = rw::call(a::hitShapeInfo_isIntersects, {shape, f.addr(0x18), f.addr(0x20)});
        n++;
        if (hit != 0) return 1;
        shape += 0x14;
        if (n > 2) break;
    }
    uint32_t maxX = rw::call(a::rect_getMaxX, {f.addr(0x30)});
    uint32_t minY = rw::call(a::rect_getMinY, {f.addr(0x30)});
    rw::call(a::vec2_ctor_ff, {f.addr(0x10), maxX, minY});
    return rw::call(a::hitShapeInfo_isIntersects, {a::touchUiShapeLast, f.addr(0x10), f.addr(0x20)});
}
RW_FUNCTION(0x757E74, "StageFieldManager::isHitTestTouchRectAndTouchUI", StageFieldManager::isHitTestTouchRectAndTouchUI);

// onEventOccursTreasure $_0 {-, this}(MapTreasure*): the opened treasure's map nodes (+0x2AC..+0x2B0) added to the
// field; returns the last call's r0 (or the closure when there are none), as the original
static uint32_t treasureNodesAdded(uint32_t self, uint32_t treasurep) {
    const uint32_t t = ld(treasurep);
    const uint32_t end = ld(t + 0x2B0);
    uint32_t r = self;
    for (uint32_t p = ld(t + 0x2AC); p != end; p += 4) r = rw::call(0x755509, {ld(self + 4), ld(p)});
    return r;
}
RW_FUNCTION(0x75842E, "StageFieldManager::onEventOccursTreasure $_0 (sub_75842E)", treasureNodesAdded);
