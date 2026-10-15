#include "MapGateMarker.hpp"

namespace {
namespace g {
constexpr uint32_t sprite_create = 0x8A637D;      // cocos2d::Sprite::create(const std::string&)
constexpr uint32_t vec2_ctor_xy = 0x8E4B9D;       // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t moveTo_create = 0x86941D;      // cocos2d::MoveTo::create(float, const Vec2&)
constexpr uint32_t easeIn_create = 0x860571;      // cocos2d::EaseIn::create(ActionInterval*, float)
constexpr uint32_t easeOut_create = 0x8606A5;     // cocos2d::EaseOut::create(ActionInterval*, float)
constexpr uint32_t sequence_create = 0x867999;    // cocos2d::Sequence::create(FiniteTimeAction*, ...)
constexpr uint32_t spawn_create = 0x8682F5;       // cocos2d::Spawn::create(FiniteTimeAction*, ...)
constexpr uint32_t repeatForever_create = 0x86815D;  // cocos2d::RepeatForever::create(ActionInterval*)
constexpr uint32_t hide_create = 0x8665E9;        // cocos2d::Hide::create()
constexpr uint32_t show_create = 0x866591;        // cocos2d::Show::create()
constexpr uint32_t place_create = 0x866A49;       // cocos2d::Place::create(const Vec2&)
constexpr uint32_t fadeTo_create = 0x86B3E1;      // cocos2d::FadeTo::create(float, GLubyte)
constexpr uint32_t fadeOut_create = 0x86B25D;     // cocos2d::FadeOut::create(float)
constexpr uint32_t delayTime_create = 0x86852D;   // cocos2d::DelayTime::create(float)
constexpr uint32_t str_nextMap = 0xE021BB;        // "map/next_map.png" (16 characters)
}  // namespace g

using actor::fb;
uint32_t ga(const void *p) { return rw::guest_addr(p); }

// A Vec2 temporary on the guest stack.
uint32_t vec2(const bt::GuestFrame &f, uint32_t off, float x, float y) {
    rw::call(g::vec2_ctor_xy, {f.addr(off), fb(x), fb(y)});   // cocos2d::Vec2::Vec2(float, float)
    return f.addr(off);
}
}  // namespace

MapGateMarker *MapGateMarker::create() {
    auto *self = rw::host_ptr<MapGateMarker>(rw::call(bt::fn::operator_new, {SIZE}));
    MapNode::construct(self);
    self->vtable = VTABLE;
    self->arrowRoot = 0;
    self->arrowFront = 0;
    self->arrowGhost = 0;
    if (init(self) == 1) {                                   // virtual init() (slot 146 = MapGateMarker::init)
        rw::call(actor_fn::ref_autorelease, {ga(self)});
        return self;
    }
    rw::vcall(self, node_slot::DELETING_DTOR, {ga(self)});
    return nullptr;
}
RW_FUNCTION(0x5FFBCC, "MapGateMarker::create", MapGateMarker::create);

MapGateMarker *MapGateMarker::destruct(MapGateMarker *self) {
    self->vtable = VTABLE;
    rw::call(actor_fn::node_removeAllChildrenWithCleanup, {ga(self), 1});
    MapNode::destruct(self);
    return self;
}
RW_FUNCTION(0x5DB1F8, "MapGateMarker::~MapGateMarker", MapGateMarker::destruct);

void MapGateMarker::destructDelete(MapGateMarker *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5DB240, "MapGateMarker::~MapGateMarker (deleting)", MapGateMarker::destructDelete);

// Hidden gate-kind node with the two arrow sprites under a holder node 80 px down; drawn above everything.
uint32_t MapGateMarker::init(MapGateMarker *self) {
    if (MapNode::init(self) != 1) return 0;
    rw::vcall(self, node_slot::SET_TAG, {ga(self), uint32_t(NODE_TAG)});
    rw::vcall(self, node_slot::SET_VISIBLE, {ga(self), 0});
    uint32_t root = rw::call(actor_fn::node_create, {});     // cocos2d::Node::create
    self->arrowRoot = root;
    actor::vcallAt(root, node_slot::SET_POSITION_Y, {root, fb(-80.0f)});
    rw::vcall(self, node_slot::ADD_CHILD, {ga(self), self->arrowRoot});

    bt::GuestFrame f(16);
    actor::makeString(f.addr(0), g::str_nextMap, 16);
    self->arrowFront = rw::call(g::sprite_create, {f.addr(0)});   // cocos2d::Sprite::create
    actor::freeString(f.addr(0));
    if (self->arrowFront)
        actor::vcallAt(self->arrowRoot, node_slot::ADD_CHILD_Z, {self->arrowRoot, self->arrowFront, 1});
    actor::makeString(f.addr(0), g::str_nextMap, 16);
    self->arrowGhost = rw::call(g::sprite_create, {f.addr(0)});
    actor::freeString(f.addr(0));
    if (self->arrowGhost)
        actor::vcallAt(self->arrowRoot, node_slot::ADD_CHILD_Z, {self->arrowRoot, self->arrowGhost, 2});
    MapNode::lockMapPosZ(self, 3.40282347e38f);              // FLT_MAX: always on top
    return 1;
}
RW_FUNCTION(0x5DB24C, "MapGateMarker::init", MapGateMarker::init);

// Start the arrow animations: the z-2 sprite bobs between y 40 and 0 forever; the z-1 sprite repeatedly reappears
// at y 40 at half opacity and drifts up to 80 while fading out. Then show the marker.
void MapGateMarker::show(MapGateMarker *self) {
    bt::GuestFrame f(32);
    uint32_t moveUp = rw::call(g::moveTo_create, {fb(0.25f), vec2(f, 0x18, 0.0f, 40.0f)});
    uint32_t easeIn = rw::call(g::easeIn_create, {moveUp, fb(2.0f)});
    uint32_t moveDown = rw::call(g::moveTo_create, {fb(0.5f), vec2(f, 0x10, 0.0f, 0.0f)});
    uint32_t bob = rw::call(g::sequence_create, {easeIn, moveDown, 0});
    uint32_t bobForever = rw::call(g::repeatForever_create, {bob});

    uint32_t hide = rw::call(g::hide_create, {});
    uint32_t place = rw::call(g::place_create, {vec2(f, 0x08, 0.0f, 40.0f)});
    uint32_t half = rw::call(g::fadeTo_create, {fb(0.0f), 128});
    uint32_t delay = rw::call(g::delayTime_create, {fb(0.25f)});
    uint32_t show = rw::call(g::show_create, {});
    uint32_t rise = rw::call(g::moveTo_create, {fb(0.5f), vec2(f, 0x00, 0.0f, 80.0f)});
    uint32_t easeOut = rw::call(g::easeOut_create, {rise, fb(2.0f)});
    uint32_t fadeOut = rw::call(g::fadeOut_create, {fb(0.5f)});
    uint32_t spawn = rw::call(g::spawn_create, {easeOut, fadeOut, 0});
    uint32_t ghost = rw::call(g::sequence_create, {hide, place, half, delay, show, spawn, 0});
    uint32_t ghostForever = rw::call(g::repeatForever_create, {ghost});

    rw::call(actor_fn::node_runAction, {self->arrowGhost, bobForever});
    rw::call(actor_fn::node_runAction, {self->arrowFront, ghostForever});
    rw::vcall(self, node_slot::SET_VISIBLE, {ga(self), 1});
}
RW_FUNCTION(0x5DB3C0, "MapGateMarker::show()", MapGateMarker::show);

void MapGateMarker::showAt(MapGateMarker *self, const bt::Vec2 *position, float rotation) {
    rw::vcall(self, node_slot::SET_ROTATION, {ga(self), fb(rotation)});
    float x = position->x, y = position->y;
    self->mapPosition.y = y;
    self->mapPosition.x = x;
    show(self);
}
RW_FUNCTION(0x5DB564, "MapGateMarker::show(Vec2&, float)", MapGateMarker::showAt);
