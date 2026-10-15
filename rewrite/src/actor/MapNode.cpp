#include "MapNode.hpp"

namespace {
uint32_t ga(const void *p) { return rw::guest_addr(p); }
}  // namespace

MapNode *MapNode::construct(MapNode *self) {
    rw::call(actor_fn::node_ctor, {ga(self)});                // cocos2d::Node::Node
    self->vtable = VTABLE;
    self->mapPosition.x = rw::load<float>(actor_fn::vec2_ZERO);   // Vec2(Vec2::ZERO)
    self->mapPosition.y = rw::load<float>(actor_fn::vec2_ZERO + 4);
    self->moveNode = 0;
    self->moveNodeLast.x = 0.0f;                              // Vec2()
    self->moveNodeLast.y = 0.0f;
    uint32_t node = rw::call(actor_fn::node_create, {});      // cocos2d::Node::create
    self->moveNode = node;
    rw::vcall(self, node_slot::ADD_CHILD, {ga(self), node});
    self->lockedZ = UNLOCKED_Z;
    return self;
}
RW_FUNCTION(0x5DA2D4, "MapNode::MapNode", MapNode::construct);

MapNode *MapNode::destruct(MapNode *self) {
    self->vtable = VTABLE;
    // (the Vec2 destructors are empty); ~Node returns this
    return rw::host_ptr<MapNode>(rw::call(actor_fn::node_dtor, {ga(self)}));   // cocos2d::Node::~Node
}
RW_FUNCTION(0x5DA38C, "MapNode::~MapNode", MapNode::destruct);

void MapNode::destructDelete(MapNode *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5DA3CC, "MapNode::~MapNode (deleting)", MapNode::destructDelete);

uint32_t MapNode::init(MapNode *) { return 1; }
RW_FUNCTION(0x5DA410, "MapNode::init", MapNode::init);

// Run a movement action on the move node, restarting it from (0, 0).
uint32_t MapNode::move(MapNode *self, uint32_t action) {
    rw::vcall(rw::host_ptr<void>(self->moveNode), node_slot::SET_POSITION, {self->moveNode, actor_fn::vec2_ZERO});
    self->moveNodeLast.y = rw::load<float>(actor_fn::vec2_ZERO + 4);
    self->moveNodeLast.x = rw::load<float>(actor_fn::vec2_ZERO);
    return rw::call(actor_fn::node_runAction, {self->moveNode, action});   // cocos2d::Node::runAction
}
RW_FUNCTION(0x5DA414, "MapNode::move", MapNode::move);

// Apply the move node's displacement since the last call to the map position (map y points down). Returns the
// number of actions still running on the move node.
uint32_t MapNode::updateMapPosition(MapNode *self) {
    float mx = self->mapPosition.x, my = self->mapPosition.y;
    uint32_t p = rw::vcall(rw::host_ptr<void>(self->moveNode), node_slot::GET_POSITION, {self->moveNode});
    float nx = rw::load<float>(p), ny = rw::load<float>(p + 4);
    float dx = nx - self->moveNodeLast.x;                     // Vec2::subtract
    float dy = ny - self->moveNodeLast.y;
    mx = dx + mx;
    my = my - dy;
    self->mapPosition.x = mx;
    self->mapPosition.y = my;
    self->moveNodeLast.x = nx;
    self->moveNodeLast.y = ny;
    return rw::call(actor_fn::node_getNumberOfRunningActions, {self->moveNode});
}
RW_FUNCTION(0x5DA460, "MapNode::updateMapPosition", MapNode::updateMapPosition);

float MapNode::getMapPosZ(MapNode *self) {
    float z = self->lockedZ;
    if (z == UNLOCKED_Z) z = self->mapPosition.y;
    return z;
}
RW_FUNCTION(0x5DA54C, "MapNode::getMapPosZ", MapNode::getMapPosZ);

MapNode *MapNode::lockMapPosZ(MapNode *self, float z) {
    self->lockedZ = z;
    return self;
}
RW_FUNCTION(0x5DA574, "MapNode::lockMapPosZ", MapNode::lockMapPosZ);

MapNode *MapNode::unlockMapPosZ(MapNode *self) {
    self->lockedZ = UNLOCKED_Z;
    return self;
}
RW_FUNCTION(0x5DA380, "MapNode::unlockMapPosZ", MapNode::unlockMapPosZ);

uint32_t MapNode::pause(MapNode *self) {
    rw::call(actor_fn::node_pause, {self->moveNode});
    return rw::call(actor_fn::node_pause, {ga(self)});
}
RW_FUNCTION(0x5DA57C, "MapNode::pause", MapNode::pause);

uint32_t MapNode::resume(MapNode *self) {
    rw::call(actor_fn::node_resume, {self->moveNode});
    return rw::call(actor_fn::node_resume, {ga(self)});
}
RW_FUNCTION(0x5DA596, "MapNode::resume", MapNode::resume);
