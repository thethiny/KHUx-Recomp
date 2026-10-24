// MapPlacementObject: a static decoration placed on the quest map (a sprite, or an LWF animation when the file name
// ends in ".lwf"), at a map position with a locked depth.
#include "actor/MapNode.hpp"

struct MapPlacementObject : MapNode {
    // what to place (the argument of create / init)
    struct Info {
        bt::Vec2 position;                 // +0  map position
        rw::Field<float> z;                // +8  locked depth
        rw::Field<uint32_t> file;          // +12 const char*: image / LWF file
        rw::Field<uint8_t> cascadeOpacity; // +16
    };

    rw::Field<uint32_t> sprite;            // +648 cocos2d::Sprite* (image placements)
    rw::Field<uint32_t> lwf;               // +652 cocos2d::LWFNode* (.lwf placements)

    static constexpr uint32_t VTABLE = 0xF929DC;   // _ZTV18MapPlacementObject + 8
    static constexpr uint32_t SIZE = 0x290;
    static constexpr int32_t NODE_TAG = 9;         // node kind on the field: placement object

    static MapPlacementObject *destruct(MapPlacementObject *self);
    static void destructDelete(MapPlacementObject *self);
    static uint32_t init(MapPlacementObject *self, const Info *info);
    static uint32_t getPlacementType(uint32_t unusedThis, uint32_t file);
    static MapPlacementObject *create(const Info *info);
};
RW_LAYOUT(MapPlacementObject::Info, file, 12);
RW_LAYOUT(MapPlacementObject::Info, cascadeOpacity, 16);
RW_LAYOUT(MapPlacementObject, sprite, 648);
RW_LAYOUT(MapPlacementObject, lwf, 652);
RW_SIZE(MapPlacementObject, 0x290);

namespace {
using rw::guest_addr;
using actor::vcallAt;
constexpr uint32_t MapNode_ctor = 0x5DA2D5;
constexpr uint32_t MapNode_dtor = 0x5DA38D;
constexpr uint32_t MapNode_init = 0x5DA411;
constexpr uint32_t MapNode_lockMapPosZ = 0x5DA575;
constexpr uint32_t LWFNode_create = rwlib::cocos2d::LWFNode::create;          // cocos2d::LWFNode::create(const char*, void*)
constexpr uint32_t LWFUtil_preloadResources = 0x491361; // (const std::string&)
constexpr uint32_t Sprite_create = rwlib::cocos2d::Sprite::create__RKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE;           // cocos2d::Sprite::create(const std::string&)
constexpr uint32_t Rect_copy = rwlib::cocos2d::Rect::ctor_base__RKS0_;               // cocos2d::Rect::Rect(const Rect&)
constexpr uint32_t Rect_getMidX = rwlib::cocos2d::Rect::getMidX;
constexpr uint32_t Rect_getMidY = rwlib::cocos2d::Rect::getMidY;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t STRRCHR = 0x41A560;                 // strrchr (import)
constexpr uint32_t MEMCMP = 0x40EF2C;                  // memcmp (import)
constexpr uint32_t S_LWF = 0xE02256;                   // "lwf"
constexpr uint32_t SPRITE_RECT = 0x358;                // cocos2d::Sprite::_rect
}  // namespace

MapPlacementObject *MapPlacementObject::destruct(MapPlacementObject *self) {
    self->vtable = VTABLE;
    rw::call(actor_fn::node_removeAllChildrenWithCleanup, {guest_addr(self), 1});
    rw::call(MapNode_dtor, {guest_addr(self)});
    return self;
}
RW_FUNCTION(0x5DC55C, "MapPlacementObject::~MapPlacementObject", MapPlacementObject::destruct);

void MapPlacementObject::destructDelete(MapPlacementObject *self) {
    guest::deleteObject(guest_addr(destruct(self)));
}
RW_FUNCTION(0x5DC5A4, "MapPlacementObject::~MapPlacementObject (deleting)", MapPlacementObject::destructDelete);

uint32_t MapPlacementObject::init(MapPlacementObject *self, const Info *info) {
    const uint32_t me = guest_addr(self);
    if (rw::call(MapNode_init, {me}) != 1) return 0;
    uint32_t tagResult = vcallAt(me, node_slot::SET_TAG, {me, uint32_t(NODE_TAG)});
    const uint32_t file = info->file;
    if (getPlacementType(tagResult, file) == 1) {
        uint32_t node = rw::call(LWFNode_create, {file, 0});
        self->lwf = node;
        if (node) {
            self->mapPosition.y = info->position.y;
            self->mapPosition.x = info->position.x;
            vcallAt(node, node_slot::SET_POSITION, {node, actor_fn::vec2_ZERO});
            vcallAt(me, node_slot::ADD_CHILD_Z, {me, self->lwf, 1});
            guest::Frame f(12);
            const uint32_t name = f.addr();
            actor::makeString(name, info->file, uint32_t(rw::call(actor_fn::strlen_, {info->file})));
            rw::call(LWFUtil_preloadResources, {name});
            actor::freeString(name);
        }
    } else {
        guest::Frame f(0x20);
        const uint32_t name = f.addr(0x10);
        actor::makeString(name, file, uint32_t(rw::call(actor_fn::strlen_, {file})));
        uint32_t sprite = rw::call(Sprite_create, {name});
        self->sprite = sprite;
        if (rw::load<uint8_t>(name) & 1) {
            guest::deleteObject(rw::load<uint32_t>(name + 8));
            sprite = self->sprite;
        }
        if (sprite) {
            self->mapPosition.y = info->position.y;
            self->mapPosition.x = info->position.x;
            const uint32_t rect = f.addr(0);
            rw::call(Rect_copy, {rect, sprite + SPRITE_RECT});
            const uint32_t s = self->sprite;
            const uint32_t setPositionXY = rw::load<uint32_t>(rw::load<uint32_t>(s) + node_slot::SET_POSITION_XY * 4);
            uint32_t midX = rw::call(Rect_getMidX, {rect});
            uint32_t midY = rw::call(Rect_getMidY, {rect});
            rw::call(setPositionXY, {s, midX, midY ^ 0x80000000u});   // (midX, -midY)
            vcallAt(me, node_slot::ADD_CHILD_Z, {me, self->sprite, 1});
            rw::call(Vec2_dtor, {rect});
        }
    }
    vcallAt(me, node_slot::SET_CASCADE_OPACITY, {me, info->cascadeOpacity});
    rw::call(MapNode_lockMapPosZ, {me, rw::fbits(info->z)});
    return 1;
}
RW_FUNCTION(0x5DC5B0, "MapPlacementObject::init", MapPlacementObject::init);

// 1 when the file's extension starts with "lwf".
uint32_t MapPlacementObject::getPlacementType(uint32_t, uint32_t file) {
    uint32_t dot = rw::call(STRRCHR, {file, '.'});
    if (!dot) return 0;
    return rw::call(MEMCMP, {dot + 1, S_LWF, 3}) == 0 ? 1 : 0;
}
RW_FUNCTION(0x5DC7B0, "MapPlacementObject::getPlacementType", MapPlacementObject::getPlacementType);

MapPlacementObject *MapPlacementObject::create(const Info *info) {
    uint32_t p = guest::newObject(SIZE);
    rw::call(MapNode_ctor, {p});
    MapPlacementObject *self = rw::host_ptr<MapPlacementObject>(p);
    self->vtable = VTABLE;
    self->sprite = 0;
    self->lwf = 0;
    if (init(self, info) == 1) {
        rw::call(actor_fn::ref_autorelease, {p});
        return self;
    }
    vcallAt(p, node_slot::DELETING_DTOR, {p});
    return nullptr;
}
RW_FUNCTION(0x5DC7F4, "MapPlacementObject::create", MapPlacementObject::create);
