#include "MapPrize.hpp"

namespace {
namespace g {
constexpr uint32_t sprite_create = 0x8A637D;         // cocos2d::Sprite::create(const std::string&)
constexpr uint32_t stringBuffer_ctor = 0x49140D;     // StringBuffer::StringBuffer(const char* fmt, ...)
constexpr uint32_t random_getInstance = 0x492055;    // Random::getInstance()
constexpr uint32_t random_uniformFloat = 0x492261;   // Random::generateUniformFloat(float, float, unsigned*)
constexpr uint32_t function_assign = 0x5DD16D;       // std::function<void(MapPrize*)>::operator=(const function&)
constexpr uint32_t function_call = 0x5DD4B5;         // std::function<void(MapPrize*)>::operator()(MapPrize*)
constexpr uint32_t vec2_ctor_xy = 0x8E4B9D;          // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t moveTo_create = 0x86941D;         // cocos2d::MoveTo::create(float, const Vec2&)
constexpr uint32_t easeIn_create = 0x860571;         // cocos2d::EaseIn::create(ActionInterval*, float)
constexpr uint32_t easeInOut_create = 0x8607D9;      // cocos2d::EaseInOut::create(ActionInterval*, float)
constexpr uint32_t scaleBy_create = 0x86AC71;        // cocos2d::ScaleBy::create(float, float)
constexpr uint32_t rotateBy_create = 0x868B9F;       // cocos2d::RotateBy::create(float, float)
constexpr uint32_t callFunc_create = 0x866B39;       // cocos2d::CallFunc::create(const std::function<void()>&)
constexpr uint32_t sequence_create = 0x867999;       // cocos2d::Sequence::create(FiniteTimeAction*, ...)
constexpr uint32_t spawn_create = 0x8682F5;          // cocos2d::Spawn::create(FiniteTimeAction*, ...)
constexpr uint32_t repeatForever_create = 0x86815D;  // cocos2d::RepeatForever::create(ActionInterval*)
constexpr uint32_t convertToWorldSpace = 0x895A39;   // cocos2d::Node::convertToWorldSpace(const Vec2&) const (sret)
constexpr uint32_t blendAdditive = 0xEBBD8C;         // cocos2d::BlendFunc::ADDITIVE
constexpr uint32_t spriteFiles = 0xF5D91C;           // const char*[12]: img/prizes/Prize_Money01.png ...
constexpr uint32_t str_materialFmt = 0xDF9101;       // "img/material/material_%d.png"
// lambdas
constexpr uint32_t attractLambda = 0xF5D954;         // setAbsorptionAttract $_1 (captures this)
constexpr uint32_t attractLambdaTI = 0xF92EF4;
constexpr uint32_t attractLambdaTS = 0xE4CBC0;
constexpr uint32_t vortexLambda = 0xF5D980;          // setAbsorptionVortex $_0 (captures this)
constexpr uint32_t vortexLambdaTI = 0xF92F08;
constexpr uint32_t vortexLambdaTS = 0xE4CCB0;
}  // namespace g

constexpr uint32_t SPRITE_SET_BLEND_FUNC = 174;      // cocos2d::Sprite::setBlendFunc(const BlendFunc&)
constexpr uint32_t NODE_SET_USER_DATA = 88;          // cocos2d::Node::setUserData(void*)
constexpr uint32_t NODE_TRANSFORM_UPDATED = 236;     // cocos2d::Node byte flags cleared by breakAwayArmature
constexpr uint32_t NODE_TRANSFORM_DIRTY_2 = 372;

using actor::fb;
uint32_t ga(const void *p) { return rw::guest_addr(p); }

// The Gift as a 16-byte scratch Item on the guest stack: decoded, then rewritten in full (padding included).
void readGift(MapPrize *self, uint32_t tmp) {
    SafeBuffer::read(&self->gift, rw::host_ptr<uint8_t>(tmp), self->gift.data.get(), 16, 0);
}
void writeGift(MapPrize *self, uint32_t tmp) {
    SafeBuffer::write(&self->gift, self->gift.data.get(), rw::host_ptr<uint8_t>(tmp), 16, 0);
}
float randomJitter() {
    uint32_t r = rw::call(g::random_getInstance, {});
    return rw::callf(g::random_uniformFloat, {r, fb(-0.1f), fb(0.1f), 0});
}
uint32_t vec2(const bt::GuestFrame &f, uint32_t off, float x, float y) {
    rw::call(g::vec2_ctor_xy, {f.addr(off), fb(x), fb(y)});
    return f.addr(off);
}
}  // namespace

MapPrize *MapPrize::construct(MapPrize *self) {
    MapNode::construct(self);
    self->vtable = VTABLE;
    SafeBuffer::construct(&self->gift, 16, 1);
    self->gift.vtable = Gift::VTABLE;
    self->onAbsorbed.f = 0;
    self->material = 0;
    rw::store16(&self->ready, 0);                            // ready, absorbed
    self->sprite = 0;
    self->bone = 0;
    return self;
}
RW_FUNCTION(0x5DCB48, "MapPrize::MapPrize", MapPrize::construct);

MapPrize *MapPrize::destruct(MapPrize *self) {
    self->vtable = VTABLE;
    rw::call(actor_fn::node_removeAllChildrenWithCleanup, {ga(self), 1});
    fnthunk::destroy(ga(&self->onAbsorbed));                 // ~std::function
    SafeBuffer::destruct(&self->gift);
    MapNode::destruct(self);
    return self;
}
RW_FUNCTION(0x5DC870, "MapPrize::~MapPrize", MapPrize::destruct);

void MapPrize::destructDelete(MapPrize *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5DC914, "MapPrize::~MapPrize (deleting)", MapPrize::destructDelete);

void MapPrize::Gift::destructDelete(Gift *self) {
    SafeBuffer::destruct(self);
    rw::call(bt::fn::operator_delete, {ga(self)});
}
RW_FUNCTION(0x5DD7C4, "MapPrize::Gift::~Gift (deleting)", MapPrize::Gift::destructDelete);

// An item / munny / Lux icon (money and HP / CP icons drawn at 85%, Lux additive).
uint32_t MapPrize::initId(MapPrize *self, const bt::Vec2 *position, uint32_t id, float scale) {
    uint32_t failed = MapNode::init(self) ^ 1;
    if (id > ID_MAX || failed != 0) return 0;
    rw::vcall(self, node_slot::SET_TAG, {ga(self), uint32_t(NODE_TAG)});
    uint32_t file = actor::ld(g::spriteFiles + id * 4);
    bt::GuestFrame f(16);
    actor::makeString(f.addr(0), file, rw::call(actor_fn::strlen_, {file}));
    self->sprite = rw::call(g::sprite_create, {f.addr(0)});  // cocos2d::Sprite::create
    actor::freeString(f.addr(0));
    float s = id > 5 ? scale : scale * 0.85f;
    actor::vcallAt(self->sprite, node_slot::SET_SCALE, {self->sprite, fb(s)});
    if ((id & ~1u) == 10) actor::vcallAt(self->sprite, SPRITE_SET_BLEND_FUNC, {self->sprite, g::blendAdditive});
    rw::vcall(self, node_slot::ADD_CHILD, {ga(self), self->sprite});
    rw::vcall(self, node_slot::SET_VISIBLE, {ga(self), 1});
    float x = position->x, y = position->y;
    self->mapPosition.y = y;
    self->mapPosition.x = x;
    return 1;
}
RW_FUNCTION(0x5DC920, "MapPrize::init(Vec2, ID, float)", MapPrize::initId);

MapPrize *MapPrize::createId(const bt::Vec2 *position, uint32_t id, float scale) {
    auto *self = rw::host_ptr<MapPrize>(rw::call(bt::fn::operator_new, {SIZE}));
    construct(self);
    bt::GuestFrame f(8);                                     // the Vec2 argument (by value)
    rw::save<float>(f.addr(0), position->x);
    rw::save<float>(f.addr(4), position->y);
    if (rw::vcall(self, VSLOT_INIT_ID, {ga(self), f.addr(0), id, fb(scale)}) == 1) {
        rw::call(actor_fn::ref_autorelease, {ga(self)});
        return self;
    }
    rw::vcall(self, node_slot::DELETING_DTOR, {ga(self)});
    return nullptr;
}
RW_FUNCTION(0x5DCA84, "MapPrize::create(Vec2, ID, float)", MapPrize::createId);

// A material icon (img/material/material_<id>.png at 80%).
uint32_t MapPrize::initMaterial(MapPrize *self, const bt::Vec2 *position, int32_t materialId) {
    if (MapNode::init(self) != 1) return 0;
    rw::vcall(self, node_slot::SET_TAG, {ga(self), uint32_t(NODE_TAG)});
    enum : uint32_t { STR = 0, BUF = 16 };
    bt::GuestFrame f(16 + 0x420);
    rw::call(g::stringBuffer_ctor, {f.addr(BUF), g::str_materialFmt, uint32_t(materialId)});   // StringBuffer
    actor::makeString(f.addr(STR), f.addr(BUF), rw::call(actor_fn::strlen_, {f.addr(BUF)}));
    self->sprite = rw::call(g::sprite_create, {f.addr(STR)});
    actor::freeString(f.addr(STR));
    if (!self->sprite) return 0;
    self->material = 1;
    actor::vcallAt(self->sprite, node_slot::SET_SCALE, {self->sprite, fb(0.8f)});
    rw::vcall(self, node_slot::ADD_CHILD, {ga(self), self->sprite});
    rw::vcall(self, node_slot::SET_VISIBLE, {ga(self), 1});
    float x = position->x, y = position->y;
    self->mapPosition.y = y;
    self->mapPosition.x = x;
    return 1;
}
RW_FUNCTION(0x5DCBC8, "MapPrize::init(Vec2, int)", MapPrize::initMaterial);

MapPrize *MapPrize::createMaterial(const bt::Vec2 *position, int32_t materialId) {
    auto *self = rw::host_ptr<MapPrize>(rw::call(bt::fn::operator_new, {SIZE}));
    construct(self);
    bt::GuestFrame f(8);
    rw::save<float>(f.addr(0), position->x);
    rw::save<float>(f.addr(4), position->y);
    if (rw::vcall(self, VSLOT_INIT_MATERIAL, {ga(self), f.addr(0), uint32_t(materialId)}) == 1) {
        rw::call(actor_fn::ref_autorelease, {ga(self)});
        return self;
    }
    rw::vcall(self, node_slot::DELETING_DTOR, {ga(self)});
    return nullptr;
}
RW_FUNCTION(0x5DCD1C, "MapPrize::create(Vec2, int)", MapPrize::createMaterial);

// Store the prize's contents (or "nothing": type 1). withoutCount stores a count of 0.
void MapPrize::setItem(MapPrize *self, const Item *item, uint32_t withoutCount) {
    bt::GuestFrame f(16);
    readGift(self, f.addr(0));
    auto *tmp = f.ptr<Item>(0);
    if (item) {
        rw::save<int32_t>(f.addr(0), item->type);
        rw::save<int32_t>(f.addr(4), item->id);
        rw::save<int32_t>(f.addr(8), withoutCount ? 0 : int32_t(item->count));
        rw::save<uint8_t>(f.addr(12), item->rare);
    } else {
        rw::save<int32_t>(f.addr(0), 1);
        rw::save<int32_t>(f.addr(4), 0);
        rw::save<int32_t>(f.addr(8), 0);
        rw::save<uint8_t>(f.addr(12), 0);
    }
    (void)tmp;
    writeGift(self, f.addr(0));
}
RW_FUNCTION(0x5DCDDC, "MapPrize::setItem", MapPrize::setItem);

// Copy the contents out when it is a real item (type 2..20); clear == 1 empties the prize. Returns 1 if copied.
uint32_t MapPrize::getItem(MapPrize *self, Item *out, uint32_t clear) {
    bt::GuestFrame f(16);
    readGift(self, f.addr(0));
    uint32_t got = 0;
    int32_t type = rw::load<int32_t>(f.addr(0));
    if (uint32_t(type - 2) <= 18) {
        out->type = type;
        out->id = rw::load<int32_t>(f.addr(4));
        out->count = rw::load<int32_t>(f.addr(8));
        out->rare = rw::load<uint8_t>(f.addr(12));
        got = 1;
    }
    if (clear == 1) {
        rw::save<int32_t>(f.addr(0), 1);
        rw::save<int32_t>(f.addr(4), 0);
        rw::save<int32_t>(f.addr(8), 0);
        rw::save<uint8_t>(f.addr(12), 0);
        writeGift(self, f.addr(0));
    }
    return got;
}
RW_FUNCTION(0x5DCE94, "MapPrize::getItem", MapPrize::getItem);

// Fly into the player: start at `from` (relative to the player), swing out to (-0.6x, 0.6y), back through
// (0.3x, 0.3y) to the centre, then onAbsorptFinish.
void MapPrize::setAbsorptionVortex(MapPrize *self, const bt::Vec2 *from, uint32_t onAbsorbed) {
    float jitter = randomJitter();
    rw::call(g::function_assign, {ga(&self->onAbsorbed), onAbsorbed});
    rw::vcall(self, node_slot::SET_POSITION, {ga(self), ga(from)});
    enum : uint32_t { V1 = 0, V2 = 8, V3 = 16, FN = 24 };
    bt::GuestFrame f(48);
    float x1 = from->x * -0.6f, y1 = from->y * 0.6f;
    uint32_t m1 = rw::call(g::moveTo_create, {fb(jitter + 0.25f), vec2(f, V1, x1, y1)});
    uint32_t e1 = rw::call(g::easeInOut_create, {m1, fb(2.0f)});
    float x2 = from->x * 0.3f, y2 = from->y * 0.3f;
    uint32_t m2 = rw::call(g::moveTo_create, {fb(jitter + 0.15f), vec2(f, V2, x2, y2)});
    uint32_t e2 = rw::call(g::easeInOut_create, {m2, fb(2.0f)});
    float x3 = from->x * 0.0f, y3 = from->y * 0.0f;
    uint32_t m3 = rw::call(g::moveTo_create, {fb(jitter + 0.1f), vec2(f, V3, x3, y3)});
    uint32_t e3 = rw::call(g::easeInOut_create, {m3, fb(2.0f)});
    fnthunk::makeLocal(f.addr(FN), g::vortexLambda, {ga(self)});
    uint32_t done = rw::call(g::callFunc_create, {f.addr(FN)});
    uint32_t seq = rw::call(g::sequence_create, {e1, e2, e3, done, 0});
    fnthunk::destroy(f.addr(FN));
    rw::call(actor_fn::node_runAction, {ga(self), seq});
}
RW_FUNCTION(0x5DCF44, "MapPrize::setAbsorptionVortex", MapPrize::setAbsorptionVortex);

// Fly to a HUD target: from `from` to the centre while shrinking to nothing (spinning the icon if asked).
void MapPrize::setAbsorptionAttract(MapPrize *self, const bt::Vec2 *from, uint32_t onAbsorbed, uint32_t spin) {
    float jitter = randomJitter();
    rw::call(g::function_assign, {ga(&self->onAbsorbed), onAbsorbed});
    rw::vcall(self, node_slot::SET_POSITION, {ga(self), ga(from)});
    enum : uint32_t { V = 0, FN = 8 };
    bt::GuestFrame f(32);
    float t = jitter + 0.5f;
    uint32_t move = rw::call(g::moveTo_create, {fb(t), vec2(f, V, 0.0f, 0.0f)});
    uint32_t easeMove = rw::call(g::easeIn_create, {move, fb(2.0f)});
    uint32_t shrink = rw::call(g::scaleBy_create, {fb(t), fb(0.0f)});
    uint32_t easeShrink = rw::call(g::easeIn_create, {shrink, fb(12.0f)});
    uint32_t spawn = rw::call(g::spawn_create, {easeMove, easeShrink, 0});
    fnthunk::makeLocal(f.addr(FN), g::attractLambda, {ga(self)});
    uint32_t done = rw::call(g::callFunc_create, {f.addr(FN)});
    uint32_t seq = rw::call(g::sequence_create, {spawn, done, 0});
    fnthunk::destroy(f.addr(FN));
    rw::call(actor_fn::node_runAction, {ga(self), seq});
    if (spin == 1) {
        uint32_t rotate = rw::call(g::rotateBy_create, {fb(0.5f), fb(360.0f)});
        uint32_t forever = rw::call(g::repeatForever_create, {rotate});
        rw::call(actor_fn::node_runAction, {self->sprite, forever});
    }
}
RW_FUNCTION(0x5DD1DC, "MapPrize::setAbsorptionAttract", MapPrize::setAbsorptionAttract);

uint32_t MapPrize::setBone(MapPrize *self, uint32_t bone) {
    self->bone = bone;
    return actor::vcallAt(bone, NODE_SET_USER_DATA, {bone, ga(self)});
}
RW_FUNCTION(0x5DD378, "MapPrize::setBone", MapPrize::setBone);

// Leave the eruption armature: the map position takes the bone's world offset, the node leaves its parent.
uint32_t MapPrize::breakAwayArmature(MapPrize *self) {
    if (!self->bone) return 0;
    bt::GuestFrame f(8);
    rw::call(g::convertToWorldSpace, {f.addr(0), ga(self), actor_fn::vec2_ZERO});
    float wx = rw::load<float>(f.addr(0)), wy = rw::load<float>(f.addr(4));
    self->mapPosition.x = wx + self->mapPosition.x;
    self->mapPosition.y = self->mapPosition.y - wy;
    rw::set<uint8_t>(self, NODE_TRANSFORM_UPDATED, 0);
    rw::set<uint8_t>(self, NODE_TRANSFORM_DIRTY_2, 0);
    rw::vcall(self, node_slot::REMOVE_FROM_PARENT, {ga(self)});
    self->bone = 0;
    return 1;
}
RW_FUNCTION(0x5DD398, "MapPrize::breakAwayArmature", MapPrize::breakAwayArmature);

void MapPrize::onAbsorptFinish(MapPrize *self) {
    self->absorbed = 1;
    rw::vcall(self, node_slot::SET_VISIBLE, {ga(self), 0});
    if (!self->onAbsorbed.f) return;
    rw::call(g::function_call, {ga(&self->onAbsorbed), ga(self)});
    fnthunk::destroy(ga(&self->onAbsorbed));                 // callback = nullptr
    self->onAbsorbed.f = 0;
}
RW_FUNCTION(0x5DD454, "MapPrize::onAbsorptFinish", MapPrize::onAbsorptFinish);

// ------------------------------------------------------------------------------------------- lambdas
void MapPrize::absorbedLambda(uint32_t lambda) {
    uint32_t self = actor::ld(lambda + 4);
    rw::call(actor_fn::ref_retain, {self});
    onAbsorptFinish(rw::host_ptr<MapPrize>(self));
    rw::call(actor_fn::ref_autorelease, {self});
}

// setAbsorptionAttract $_1
RW_FUNCTION(0x5DD534, "MapPrize::setAbsorptionAttract $_1 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5DD53C, "MapPrize::setAbsorptionAttract $_1 __func::__clone()", (fnthunk::cloneWords<g::attractLambda, 8, 1>));
RW_FUNCTION(0x5DD568, "MapPrize::setAbsorptionAttract $_1 __func::__clone(__base*)", fnthunk::cloneInto1<g::attractLambda>);
RW_FUNCTION(0x5DD588, "MapPrize::setAbsorptionAttract $_1 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5DD58A, "MapPrize::setAbsorptionAttract $_1 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5DD592, "MapPrize::setAbsorptionAttract $_1 (finished: onAbsorptFinish)", MapPrize::absorbedLambda);
RW_FUNCTION(0x5DD5B0, "MapPrize::setAbsorptionAttract $_1 __func::target", fnthunk::target<g::attractLambdaTS>);
RW_FUNCTION(0x5DD5D0, "MapPrize::setAbsorptionAttract $_1 __func::target_type", fnthunk::targetType<g::attractLambdaTI>);
// setAbsorptionVortex $_0
RW_FUNCTION(0x5DD5E4, "MapPrize::setAbsorptionVortex $_0 __func::~__func (deleting)", fnthunk::deleteThis);
RW_FUNCTION(0x5DD5EC, "MapPrize::setAbsorptionVortex $_0 __func::__clone()", (fnthunk::cloneWords<g::vortexLambda, 8, 1>));
RW_FUNCTION(0x5DD618, "MapPrize::setAbsorptionVortex $_0 __func::__clone(__base*)", fnthunk::cloneInto1<g::vortexLambda>);
RW_FUNCTION(0x5DD638, "MapPrize::setAbsorptionVortex $_0 __func::destroy", fnthunk::nothing);
RW_FUNCTION(0x5DD63A, "MapPrize::setAbsorptionVortex $_0 __func::destroy_deallocate", fnthunk::deleteThis);
RW_FUNCTION(0x5DD642, "MapPrize::setAbsorptionVortex $_0 (finished: onAbsorptFinish)", MapPrize::absorbedLambda);
RW_FUNCTION(0x5DD660, "MapPrize::setAbsorptionVortex $_0 __func::target", fnthunk::target<g::vortexLambdaTS>);
RW_FUNCTION(0x5DD680, "MapPrize::setAbsorptionVortex $_0 __func::target_type", fnthunk::targetType<g::vortexLambdaTI>);
